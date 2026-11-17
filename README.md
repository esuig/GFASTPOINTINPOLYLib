# GFastPointInPolygonGrid

Fast point-in-polygon classification for C++, based on a regular grid
acceleration structure. Single header, no dependencies beyond the standard
library.

Each polygon is pre-processed once into a uniform grid. Every grid cell stores
the polygon edges crossing it and the inside/outside state of its corners, so a
query only has to look at the few edges of a single cell, instead of testing
every edge of the polygon. This makes it well suited to classifying many points
against the same polygons.

It is a modernized C++ adaptation of the grid method by Eric Haines
(*Graphics Gems IV*), extended to support polygons with holes and to detect
points lying on the polygon boundary. See [CREDITS.md](CREDITS.md).

> **Project status:** this library is at version 0.0.1, an early release. It
> is still being tested and will be further developed, so its API may change
> in future versions. Feedback and bug reports are welcome.
>
> This library has been tested only on Windows and compiled with MSVC 2022 so far
>
> A test suite for the library will be introduced soon

## Features

- **Fast queries**: after pre-processing, each query tests only the edges in
  one grid cell.
- **Multiple profiles and holes**: outer boundaries and holes are given as
  separate profiles and distinguished by their orientation.
- **Boundary detection**: points lying on a profile are reported separately
  from inside and outside points.
- **Header-only**: copy one file into your project and include it.
- **No dependencies**: standard library only (C++11 or later).

## Getting started

Copy `GFastPointInPolygonGridLib.h` into your project and include it:

```cpp
#include "GFastPointInPolygonGridLib.h"
```

Everything is in the `gfastptinpolygridlib` namespace.

## Example

A 10 x 10 square with a 4 x 4 square hole in the middle:

```cpp
#include "GFastPointInPolygonGridLib.h"
#include <iostream>
#include <vector>

using namespace gfastptinpolygridlib;

int main()
{
    std::vector<std::vector<CPoint2>> profiles =
    {
        // Outer boundary: counterclockwise, closed (last point = first point)
        { {0.0, 0.0}, {10.0, 0.0}, {10.0, 10.0}, {0.0, 10.0}, {0.0, 0.0} },
        // Hole: clockwise, closed
        { {3.0, 3.0}, {3.0, 7.0}, {7.0, 7.0}, {7.0, 3.0}, {3.0, 3.0} }
    };

    // Build the grids (32 x 32 cells per profile)
    CGFastPointInPolygonGrid grid(profiles, 32);

    std::cout << grid.IsPointInside(1.0, 1.0)  << '\n';  // 1: inside
    std::cout << grid.IsPointInside(5.0, 5.0)  << '\n';  // 0: in the hole, so outside
    std::cout << grid.IsPointInside(12.0, 5.0) << '\n';  // 0: outside
    std::cout << grid.IsPointInside(10.0, 5.0) << '\n';  // 2: on the outer boundary
}
```

## Input conventions

- **Closed profiles**: each profile must repeat its first point as its last
  point.
- **Orientation**: counterclockwise profiles are outer boundaries, clockwise
  profiles are holes. A point is inside when it is contained in more outer
  boundaries than holes.
- **Simple profiles**: each profile should be a simple (non-self-intersecting)
  polygon with at least 3 distinct vertices.
- **Lifetime**: the object keeps a reference to the input profiles, so they
  must outlive it and must not be modified while it is in use.

## Return values

`IsPointInside(x, y)` returns:

| Value | Meaning                                                                          |
| ----- | -------------------------------------------------------------------------------- |
| `0`   | The point is outside (including inside a hole).                                  |
| `1`   | The point is inside.                                                             |
| `2`   | The point lies on one of the profiles (within an absolute tolerance of `1e-10`). |

## License

The C++ adaptation is released under the
[Boost Software License 1.0](LICENSE). The portions derived from the original
Graphics Gems code remain subject to the Graphics Gems license, which permits
use in any program, product or library, commercial or non-commercial. See
[CREDITS.md](CREDITS.md) for details.

SPDX: `BSL-1.0 AND Graphics-Gems`

## Credits

Based on the grid method by Eric Haines, "Point in Polygon Strategies",
*Graphics Gems IV* (Paul S. Heckbert, ed.), Academic Press, 1994. The corner
fallback test uses Franklin Antonio's segment intersection algorithm from
*Graphics Gems III*. See [CREDITS.md](CREDITS.md).
