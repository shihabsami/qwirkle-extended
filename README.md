# Qwirkle.

## Implementation Notes
- Using `std::vector<>::reserve(26)` with initial size, better performance.
- Using `const&` parameters to prevent one additional copy.


**Additional**

- Single argument constructor made `explicit` to prevent implicit type conversion.

