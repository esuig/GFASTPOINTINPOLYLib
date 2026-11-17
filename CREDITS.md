# Credits

GFastPointInPolygonGrid is a modernized C++ adaptation of the grid method for
point-in-polygon testing by Eric Haines, published in *Graphics Gems IV*. This
file lists the original work the library is based on and the terms under which
it is used.

## Adapted code

| Library code | Original source | Author | License |
|---|---|---|---|
| `CGFastPointInPolygonGrid` (grid setup, grid test, edge records) | `GridSetup()`, `GridTest()` and `AddGridRecAlloc()` in `ptinpoly.h` / `ptinpoly.c`, Graphics Gems IV | Eric Haines, 3D/Eye Inc. | Graphics Gems license |

Original source: https://github.com/erich666/GraphicsGems (Graphics Gems IV,
point in polygon code by Eric Haines).

The original files carry no explicit copyright notice; they are credited
"by Eric Haines, 3D/Eye Inc". All rights in the original code remain with its
author.

Changes from the original include: C++ class-based design, STL containers
instead of manual memory management, support for multiple profiles with holes,
and explicit detection of points lying on the profiles.

### Graphics Gems license

The code of the Graphics Gems series is distributed under the license of the
Graphics Gems repository (SPDX identifier: `Graphics-Gems`), which permits its
use in any program, product or library, commercial or non-commercial. Its key
terms state:

> In other words, you cannot claim the text of the code as your own and resell
> it. Using the code is permitted in any program, product, or library,
> non-commercial or commercial. Giving credit is not required, though is a nice
> gesture.

and that the code comes as-is: if there are any flaws or problems with any
Gems code, nobody involved with Gems (authors, editors, publishers, or
webmasters) is to be held responsible.

The full license text is available in the `LICENSE.md` file of the Graphics
Gems repository: https://github.com/erich666/GraphicsGems

## References

- Eric Haines, "Point in Polygon Strategies", in *Graphics Gems IV*
  (Paul S. Heckbert, ed.), Academic Press, 1994. Describes the grid method
  implemented by this library.
- Franklin Antonio, "Faster Line Segment Intersection", in *Graphics Gems III*
  (David Kirk, ed.), Academic Press, 1992. The segment intersection algorithm
  used by the corner fallback test (cells in which no border is free of
  polygon edges).
