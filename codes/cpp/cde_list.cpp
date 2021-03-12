#include <algorithm>
#include <utility>
#include <memory>
#include <experimental/propagate_const>
#include <type_traits>

template <typename T> class list
{
  using storage_type = std::aligned_storage_t<sizeof(T), alignof(T)>;

public:
  using iterator = T*;
  using const_iterator = const T*;

  explicit list(std::size_t capacity) :
    capacity_{capacity}, values_{new storage_type[capacity]}
  {}

  ~list() noexcept
  {
    empty();
  }

  auto empty()
  {
    if constexpr (!std::is_trivially_destructible_v<T>)
      std::destroy(begin(), end());
    size_ = 0;
  }

  auto begin() const { return std::launder(reinterpret_cast<const T*>(values_.get())); }
  auto begin() { return std::launder(reinterpret_cast<T*>(values_.get())); }

  auto end() const { return std::launder(reinterpret_cast<const T*>(values_.get())) + size(); }
  auto end() { return std::launder(reinterpret_cast<T*>(values_.get())) + size(); }

  auto size() const { return size_; }
  auto capacity() const { return capacity_; }

  auto is_empty() const { return size() == 0; }
  auto is_full() const { return false; }

  auto insert_back(T value)
  {
    if (capacity() == size())
      expand(2 * capacity());
    ::new (std::addressof(end())) T(std::move(value));
    ++size_;
  }

  auto insert(iterator pos, T value)
  {
    if (pos == end())
      return insert_back(std::move(value));

    {
      const auto index = std::distance(begin(), pos);
      insert_back(std::move(value)); // might invalidate pos
      pos = std::next(begin(), index);
    }

    const auto b = std::make_reverse_iterator(pos);
    const auto e = std::make_reverse_iterator(end());
    std::rotate(b, std::next(b), e);
  }

  auto remove_back()
  {
    std::destroy_at(end());
    --size_;
  }

  auto remove(iterator pos)
  {
    if (std::next(pos) != end())
      std::move(std::next(pos), end(), pos);
    remove_back();
  }

  auto expand(std::size_t new_capacity)
  {
    if (new_capacity <= capacity())
      return;

    std::unique_ptr<storage_type[]> new_values{new storage_type[new_capacity]};
    std::uninitialized_move_n(begin(), size(), std::launder(reinterpret_cast<T*>(new_values.get())));
    values_ = std::move(new_values);
  }

  auto at(std::size_t i) const -> const T& { return *std::next(begin(), i); }
  auto at(std::size_t i) -> T& { return *std::next(begin(), i); }

private:
  std::size_t capacity_;
  std::size_t size_ = 0u;
  std::experimental::propagate_const<std::unique_ptr<storage_type[]>> values_;
};
