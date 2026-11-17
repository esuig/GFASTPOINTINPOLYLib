

#ifndef H_PT_IN_POLY_GRID
#define H_PT_IN_POLY_GRID
#pragma once


#include <cmath>
#include <vector>


namespace gfastptinpolygridlib
{
	/** @brief Sentinel "infinite" value used for slopes and ray distances of axis-aligned edges. */
#define HUGE				1.797693134862315e+308
	/** @brief Relative enlargement applied to the polygon bounding box when building the grid. */
#define EPSILON				1.0e-5
	/** @brief Absolute tolerance used to decide whether a query point lies on a polygon edge. */
#define EPSILON_COINC		1.0e-10
	/** @brief Left edge of the cell is crossed by the polygon.*/
#define GC_L_EDGE_HIT       0x0010
	/** @brief Right edge of the cell is crossed by the polygon*/
#define GC_R_EDGE_HIT       0x0020 
	/** @brief Bottom edge of the cell is crossed by the polygon*/
#define GC_B_EDGE_HIT       0x0040 
	/*Top edge of the cell is crossed by the polygon*/
#define GC_T_EDGE_HIT       0x0080
	/** @brief Parity of the crossings of the bottom edge*/
#define GC_B_EDGE_PARITY    0x0100 
	/** @brief Parity of the crossings of the top edge*/
#define GC_T_EDGE_PARITY    0x0200
	/** @brief Bottom-left corner is inside the polygon (else outside)*/
#define GC_BL_IN            0x0001
	/** @brief Bottom - right corner is inside the polygon(else outside)*/
#define GC_BR_IN            0x0002
	/** @brief Top-left corner is inside the polygon (else outside)*/
#define GC_TL_IN            0x0004
	/** @brief Top - right corner is inside the polygon(else outside)*/
#define GC_TR_IN            0x0008
	/** @brief Left edge is clear (meaningful after inverting the hit flags)*/
#define GC_L_EDGE_CLEAR     GC_L_EDGE_HIT
	/** @brief Right edge is clear (meaningful after inverting the hit flags)*/
#define GC_R_EDGE_CLEAR     GC_R_EDGE_HIT
	/** @brief Bottom edge is clear (meaningful after inverting the hit flags)*/
#define GC_B_EDGE_CLEAR     GC_B_EDGE_HIT
	/** @brief Top edge is clear (meaningful after inverting the hit flags)*/
#define GC_T_EDGE_CLEAR     GC_T_EDGE_HIT
	/** @brief Mask of all four edge-hit flags*/
#define GC_ALL_EDGE_CLEAR   (GC_L_EDGE_HIT | GC_R_EDGE_HIT | GC_B_EDGE_HIT | GC_T_EDGE_HIT)
	/** @brief Shoot the test ray towards the left edge*/
#define GC_AIM_L            (0<<10)
	/** @brief Shoot the test ray towards the bottom edge*/
#define GC_AIM_B            (1<<10)
	/** @brief Shoot the test ray towards the right edge*/
#define GC_AIM_R            (2<<10)
	/** @brief Shoot the test ray towards the top edge*/
#define GC_AIM_T            (3<<10)
	/** @brief No edge is clear: test against the bottom-left corner*/
#define GC_AIM_C            (4<<10)
	/** @brief Mask extracting the GC_AIM_* value from the flags*/
#define GC_AIM              0x1c00

	/**
	* @class CPoint2
	* @brief 2D point or vector with double precision coordinates
	*/
	class CPoint2
	{
	public:

		/**
		* @brief Union to allow access to the vector's data via array or named members.
		* https://www.gamedev.net/forums/topic/671481-most-efficient-way-of-designing-a-vector-class-in-3d/
		*/
		union
		{
			/**
			* Array access to coordinates(x, y)
			*/
			double m_pc[2];
			struct
			{
				/**
				* X Coordinate
				*/
				double m_x;
				/**
				* Y Coordinate
				*/
				double m_y;
			};
		};


		/**
		* @brief Default constructor, initializes coordinates to zero
		*/
		CPoint2() :
			m_x(0.0),
			m_y(0.0)
		{

		}

		/**
		* @brief Constructs a point from its coordinates
		*
		* @param f_x X coordinate
		* @param f_y Y coordinate
		*/
		CPoint2(const double& f_x,
			const double& f_y) :
			m_x(f_x),
			m_y(f_y)
		{

		}

		/**
		* @brief Copy constructor
		*
		* Copies coordinates
		*
		* @param f_p Point to copy
		*/
		CPoint2(const CPoint2& f_p) :
					m_x(f_p.m_x),
					m_y(f_p.m_y)
		{

		}

		/**
		* @brief Copy assignment operator
		*
		* Copies coordinates
		*
		* @param f_p Point to copy
		* @return Reference to this point
		*/
		CPoint2& operator=(const CPoint2& f_p)
		{
			m_pc[0] = f_p.m_pc[0];
			m_pc[1] = f_p.m_pc[1];
			return *this;
		}
	
		/**
		* @brief Computes the squared length (x^2 + y^2)
		* @return Squared length
		*/
		inline double LengthSq() const
		{
			return m_x * m_x + m_y * m_y;
		}

		/**
		* @brief Computes the Euclidean length
		* @return Length
		*/
		inline double Length() const
		{
			return std::sqrt(LengthSq());
		}

		/**
		* @brief Normalizes this vector to unit length
		*
		* If the length is zero, the vector is left unchanged
		*
		* @return Reference to this vector
		*/
		inline CPoint2& UnitizeInPlace()
		{
			const double l_length = Length();
			if (l_length > 0.0)
			{
				m_x /= l_length;
				m_y /= l_length;
			}

			return *this;
		}

		/**
		* @brief Returns a copy of this vector normalized to unit length
		*
		* If the length is zero, an unchanged copy is returned
		*
		* @return Normalized copy
		*/
		inline CPoint2 GetUnitVector() const
		{
			const double l_length = Length();
			if (l_length > 0.0)
			{
				return CPoint2(m_x / l_length, m_y / l_length);
			}

			return *this;
		}

		/**
		* @brief Returns the vector rotated 90 degrees counter-clockwise
		* @return Perpendicular vector (-y, x), same length as this vector
		*/
		inline CPoint2 GetPerpendicular() const
		{
			return CPoint2(-m_y, m_x);
		}


		/**
		* @brief Accesses a coordinate by index
		* @param f_index Coordinate index (0 = x, 1 = y, 2 = z)
		* @return Reference to the selected coordinate
		*/
		inline double& operator[](size_t f_index)
		{
			return m_pc[f_index];
		}

		/**
		* @brief Accesses a coordinate by index (const version)
		* @param f_index Coordinate index (0 = x, 1 = y, 2 = z)
		* @return Const reference to the selected coordinate
		*/
		inline const double& operator[](size_t f_index) const
		{
			return m_pc[f_index];
		}

		/**
		* @brief Component-wise addition
		* @param f_p Point to add
		* @return Sum of the two points
		*/
		inline CPoint2 operator+(const CPoint2& f_p) const
		{
			return CPoint2(m_x + f_p.m_x, m_y + f_p.m_y);
		}
		/**
		* @brief Component-wise subtraction
		* @param f_p Point to subtract
		* @return Difference of the two points
		*/
		inline CPoint2 operator-(const CPoint2& f_p) const
		{
			return CPoint2(m_x - f_p.m_x, m_y - f_p.m_y);
		}
		/**
		* @brief Invert the vector
		* @return The negated vector
		*/
		inline CPoint2 operator-() const
		{
			return CPoint2(-m_x, -m_y);
		}
		/**
		* @brief Multiplies both components by a scalar
		* @param f_scalar Scale factor
		* @return Scaled vector
		*/
		inline CPoint2 operator*(double f_scalar) const
		{
			return CPoint2(m_x * f_scalar, m_y * f_scalar);
		}
		/**
		* @brief Divides both components by a scalar
		* @param f_scalar Divisor
		* @pre f_scalar != 0
		* @return Scaled vector
		*/
		inline CPoint2 operator/(double f_scalar) const
		{
			return CPoint2(m_x / f_scalar, m_y / f_scalar);
		}
		/**
		* @brief Multiplies both components of a vector by a scalar (scalar on the left)
		* @param f_scalar Scale factor
		* @param f_p Vector to scale
		* @return Scaled vector
		*/
		inline friend CPoint2 operator*(double f_scalar, const CPoint2& f_p)
		{
			return CPoint2(f_p.m_x * f_scalar, f_p.m_y * f_scalar);
		}

	private:
	};

	/**
	* @brief Computes the dot product of two vectors
	* @param f_a First vector
	* @param f_b Second vector
	* @return a.x * b.x + a.y * b.y
	*/
	inline static double Dot(const CPoint2& f_a, const CPoint2& f_b)
	{
		return f_a.m_x * f_b.m_x + f_a.m_y * f_b.m_y;
	}

	/**
	* @brief Computes the 2D cross product of two vectors
	*
	* Equals the z-component of the 3D cross product (a.x, a.y, 0) x (b.x, b.y, 0)
	*
	* @param f_a First vector
	* @param f_b Second vector
	* @return Positive if @p f_b is counter-clockwise from @p f_a, negative if clockwise, 0 if collinear
	*/
	inline static double Cross(const CPoint2& f_a, const CPoint2& f_b)
	{
		return f_a.m_x * f_b.m_y - f_a.m_y * f_b.m_x;
	}

	/**
	* @brief Computes the squared distance between two points
	* @param f_a First point
	* @param f_b Second point
	* @return Squared distance
	*/
	inline static double DistanceSq(const CPoint2& f_a, const CPoint2& f_b)
	{
		const double l_dx = f_b.m_x - f_a.m_x;
		const double l_dy = f_b.m_y - f_a.m_y;

		return l_dx * l_dx + l_dy * l_dy;
	}

	/**
	* @brief Computes the distance between two points
	* @param f_a First point
	* @param f_b Second point
	* @return Euclidean distance
	*/
	inline static double Distance(const CPoint2& f_a, const CPoint2& f_b)
	{
		return std::sqrt(DistanceSq(f_a, f_b));
	}

	/**
	* @brief Computes the signed 2D cross product of two vectors
	*
	* Despite its name, the result is signed (it is not the absolute value):
	* it equals v1.x * v2.y - v2.x * v1.y, i.e. the same value returned by Cross().
	*
	* @param f_v1 First vector
	* @param f_v2 Second vector
	* @return Positive if @p f_v2 is counter-clockwise from @p f_v1, negative if clockwise, 0 if collinear
	* @see Cross
	*/
	inline double CrossProductMagnitude(const CPoint2& f_v1, const CPoint2& f_v2)
	{
		return (f_v1.m_pc[0] * f_v2.m_pc[1] - f_v2.m_pc[0] * f_v1.m_pc[1]);
	}

	/**
	* @brief Fast point-in-polygon classifier based on a regular grid.
	*
	* For each input profile a uniform grid is built over its bounding box.
	* Every grid cell stores the portions of the profile edges that cross it,
	* the inside/outside state of its corners and the best direction for
	* shooting a test ray. A query then only needs to test the few edges
	* stored in the single cell containing the point.
	*
	* Multiple profiles are supported: counter-clockwise profiles are treated
	* as outer boundaries and clockwise profiles as holes.
	*
	* @note The class keeps a reference to the input profiles, which must
	*       therefore outlive the object.
	*/
	class CGFastPointInPolygonGrid
	{
	public:
		/**
		* @brief Builds the acceleration grids for all the input profiles.
		*
		* @param f_input_profiles Closed profiles (first point repeated as last
		*                         point). CCW profiles are outer boundaries, CW
		*                         profiles are holes. A reference is stored.
		* @param f_resolution     Number of grid cells along X and along Y.
		*
		* @note If the grid setup fails for one profile, the grids of that
		*       profile and of all the following ones are left empty.
		*/
		CGFastPointInPolygonGrid(std::vector<std::vector<CPoint2>>& f_input_profiles,
								 const int f_resolution) :
									m_input_profiles(f_input_profiles),
									m_resolution(f_resolution)
		{
			m_grids.resize(m_input_profiles.size());
			int l_prf_id = 0;
			for (const auto& l_prf : m_input_profiles)
			{
				std::vector<CPoint2> l_prf_no_last_pt = l_prf;
				l_prf_no_last_pt.pop_back();
				if (!GridSetup(l_prf_no_last_pt, m_resolution, m_grids[l_prf_id]))
				{
					break;
				}
				l_prf_id++;
			}
		}


		/**
		* @brief Classifies a point against the whole set of input profiles.
		*
		* Each profile is tested with its grid. Containment in a CCW profile
		* counts +1, containment in a CW profile (hole) counts -1; the point
		* is inside when the total is positive.
		*
		* @param f_x X-coordinate of the query point.
		* @param f_y Y-coordinate of the query point.
		*
		* @return 0 if the point is outside,
		*         1 if the point is inside,
		*         2 if the point lies on one of the input profiles.
		*/
		int IsPointInside(const double f_x, const double f_y) const
		{

			int l_tot_inside = 0;
			for (int l_iprf = 0; l_iprf < m_input_profiles.size(); l_iprf++)
			{
				bool l_prf_is_cw = IsPrfCW(m_input_profiles[l_iprf]);
				int l_ret = GridTest(m_grids[l_iprf], f_x, f_y);

				if (l_ret == 2)
				{
					return 2;
				}
				else if ((!l_prf_is_cw) && (l_ret == 1))
				{
					l_tot_inside++;
				}
				else if (l_prf_is_cw && (l_ret == 1))
				{
					l_tot_inside--;
				}
			}

			if (l_tot_inside > 0)
			{
				l_tot_inside = 1;
			}
			else
			{
				l_tot_inside = 0;
			}

			return l_tot_inside;
		}

	private:

		/**
		* @brief Stores information about a portion of a polygon edge within a grid cell.
		*
		* Contains the starting point, bounding box, direction vector, and slope
		* information for an edge segment intersecting a grid cell.
		*/
		struct TEdgeCellRecord
		{
			/**
			* @brief Constructs an edge record with all members initialized to zero.
			*/
			TEdgeCellRecord()
			{
			}

			/** @brief X-coordinate of the edge segment's starting point. */
			double m_xa = 0.0;

			/** @brief Y-coordinate of the edge segment's starting point. */
			double m_ya = 0.0;

			/** @brief Minimum X-coordinate of the edge segment's bounding box. */
			double m_min_x = 0.0;

			/** @brief Maximum X-coordinate of the edge segment's bounding box. */
			double m_max_x = 0.0;

			/** @brief Minimum Y-coordinate of the edge segment's bounding box. */
			double m_min_y = 0.0;

			/** @brief Maximum Y-coordinate of the edge segment's bounding box. */
			double m_max_y = 0.0;

			/**
			* @brief X-component of the edge segment's displacement vector.
			*
			* Calculated as the end point's X-coordinate minus the starting
			* point's X-coordinate.
			*/
			double m_ax = 0.0;

			/**
			* @brief Y-component of the edge segment's displacement vector.
			*
			* Calculated as the end point's Y-coordinate minus the starting
			* point's Y-coordinate.
			*/
			double m_ay = 0.0;

			/**
			* @brief Slope expressed as dx/dy.
			*
			* Used to calculate the X-coordinate at a given Y-coordinate.
			* Special values may be assigned for horizontal or vertical segments.
			*/
			double m_slope = 0.0;

			/**
			* @brief Inverse slope expressed as dy/dx.
			*
			* Used to calculate the Y-coordinate at a given X-coordinate.
			* Special values may be assigned for horizontal or vertical segments.
			*/
			double m_slope_inv = 0.0;

			/**
			* @brief Retrieves the starting and ending points of the edge segment.
			*
			* The starting point is stored directly in @c xa and @c ya.
			* The ending point is reconstructed by adding the displacement
			* vector @c ax and @c ay to the starting point.
			*
			* @param[out] f_x_start X-coordinate of the starting point.
			* @param[out] f_y_start Y-coordinate of the starting point.
			* @param[out] f_x_end X-coordinate of the ending point.
			* @param[out] f_y_end Y-coordinate of the ending point.
			*/
			void GetRecordEndpoints(double& f_x_start,
									double& f_y_start,
									double& f_x_end,
									double& f_y_end) const
			{
				f_x_start = m_xa;
				f_y_start = m_ya;

				f_x_end = m_xa + m_ax;
				f_y_end = m_ya + m_ay;
			}
		};

		/**
		* @brief A single cell of the acceleration grid.
		*/
		struct TGridCell
		{
			/**
			* @brief Combination of GC_* flags: edges crossed, edge parities,
			*        corner inside/outside states and ray aim direction.
			*/
			unsigned short m_gc_flags = 0;
			/** @brief Portions of the polygon edges that fall inside this cell. */
			std::vector<TEdgeCellRecord> m_grid_cell_records;
		};

		/**
		* @brief Uniform grid built over the bounding box of one profile.
		*
		* Cells are stored row by row: the cell at column @c x and row @c y
		* has index <tt>y * m_x_res + x</tt>.
		*/
		struct TGrid
		{
			/** @brief Number of cells along X. */
			int m_x_res = 0;
			/** @brief Number of cells along Y. */
			int m_y_res = 0;
			/** @brief Minimum X of the (slightly enlarged) grid bounding box. */
			double m_min_x = 0.0;
			/** @brief Maximum X of the (slightly enlarged) grid bounding box. */
			double m_max_x = 0.0;
			/** @brief Minimum Y of the (slightly enlarged) grid bounding box. */
			double m_min_y = 0.0;
			/** @brief Maximum Y of the (slightly enlarged) grid bounding box. */
			double m_max_y = 0.0;
			/** @brief Width of a cell along X. */
			double m_cell_x_width = 0.0;
			/** @brief Width of a cell along Y. */
			double m_cell_y_width = 0.0;
			/** @brief Inverse of m_cell_x_width, used to compute cell indices. */
			double m_cell_x_width_inv = 0.0;
			/** @brief Inverse of m_cell_y_width, used to compute cell indices. */
			double m_cell_y_width_inv = 0.0;
			/** @brief X-coordinates of the vertical grid lines (m_x_res + 1 values). */
			std::vector<double> m_cells_x_values;
			/** @brief Y-coordinates of the horizontal grid lines (m_y_res + 1 values). */
			std::vector<double> m_cells_y_values;
			/** @brief Grid cells, stored row by row (m_x_res * m_y_res elements). */
			std::vector<TGridCell> m_cells;
			/** @brief Width of the polygon bounding box, before enlargement. */
			double m_grid_x_extent = 0.0;
			/** @brief Height of the polygon bounding box, before enlargement. */
			double m_grid_y_extent = 0.0;

			/**
			* @brief Resets the flags and removes the edge records of every cell.
			*
			* The grid layout and the cell count are left unchanged.
			*/
			void ClearGrid()
			{
				for (auto& l_grid_cell : m_cells)
				{
					l_grid_cell.m_gc_flags = 0;
					l_grid_cell.m_grid_cell_records.clear();
				}
			}

			/**
			* @brief Computes cell sizes and grid line coordinates from the
			*        current bounding box and resolution.
			*
			* The last grid line on each axis is set exactly to the maximum
			* bound to avoid round-off errors. Does nothing if the grid line
			* vectors have not been allocated.
			*/
			void InitializeGridLayout()
			{
				if ((m_cells_x_values.size() == 0) || (m_cells_y_values.size() == 0))
				{
					return;
				}

				m_cell_x_width = (m_max_x - m_min_x) / (double)m_x_res;
				m_cell_x_width_inv = 1.0 / m_cell_x_width;

				m_cell_y_width = (m_max_y - m_min_y) / (double)m_y_res;
				m_cell_y_width_inv = 1.0 / m_cell_y_width;

				for (int i = 0; i < m_x_res; i++)
				{
					m_cells_x_values[i] = m_min_x + i * m_cell_x_width;
				}
				m_cells_x_values[m_x_res] = m_max_x;

				for (int i = 0; i < m_y_res; i++)
				{
					m_cells_y_values[i] = m_min_y + i * m_cell_y_width;
				}
				m_cells_y_values[m_y_res] = m_max_y;
			}

			/**
			* @brief Allocates the grid and computes its bounding box.
			*
			* Sets a square resolution, allocates cells and grid line vectors,
			* computes the polygon bounding box and enlarges it by EPSILON
			* times its extent on each side.
			*
			* @param f_prf      Polygon vertices (last point not repeated).
			* @param f_grid_res Number of cells along X and along Y.
			*
			* @return Tolerance to use when checking for edges that cross a
			*         grid corner, or -1.0 if the input is invalid
			*         (@p f_grid_res < 1 or fewer than 3 vertices).
			*/
			double InitializeGridBounds(const std::vector<CPoint2>& f_prf, const int f_grid_res)
			{
				if ((f_grid_res < 1) || (f_prf.size() < 3))
				{
					return -1.0;
				}

				m_x_res = m_y_res = f_grid_res;
				m_cells_x_values.resize(m_x_res + 1);
				m_cells_y_values.resize(m_y_res + 1);
				m_cells.resize(m_x_res * m_y_res);

				m_min_x = m_max_x = f_prf[0].m_x;
				m_min_y = m_max_y = f_prf[0].m_y;
				for (size_t i = 1; i < f_prf.size(); i++)
				{
					const double l_vx0 = f_prf[i].m_x;
					if (m_min_x > l_vx0)
					{
						m_min_x = l_vx0;
					}
					else if (m_max_x < l_vx0)
					{
						m_max_x = l_vx0;
					}
					const double l_vy0 = f_prf[i].m_y;
					if (m_min_y > l_vy0)
					{
						m_min_y = l_vy0;
					}
					else if (m_max_y < l_vy0)
					{
						m_max_y = l_vy0;
					}
				}

				m_grid_x_extent = m_max_x - m_min_x;
				m_grid_y_extent = m_max_y - m_min_y;
				m_min_x -= EPSILON * m_grid_x_extent;
				m_max_x += EPSILON * m_grid_x_extent;
				m_min_y -= EPSILON * m_grid_y_extent;
				m_max_y += EPSILON * m_grid_y_extent;

				const double l_eps = 1e-9 * (m_grid_x_extent + m_grid_y_extent);

				return l_eps;
			}

			/**
			* @brief Slightly shifts the minimum corner of the bounding box.
			*
			* Used when a polygon edge passes exactly through a grid corner:
			* changing the grid alignment and rebuilding the grid avoids the
			* degenerate case.
			*/
			void MoveBBoxMin()
			{
				m_min_x -= EPSILON * m_grid_x_extent * 0.24;
				m_min_y -= EPSILON * m_grid_y_extent * 0.10;
			}

		};
		
		/**
		* @brief Determines the orientation of a closed profile.
		*
		* Uses the shoelace formula on the signed area.
		*
		* @param f_prf Closed profile (first point repeated as last point).
		*
		* @return true if the profile is clockwise, false if it is
		*         counter-clockwise or has fewer than 3 points.
		*/
		bool IsPrfCW(const std::vector<CPoint2>& f_prf) const
		{
			size_t l_prf_size = f_prf.size();
			if (l_prf_size < 3)
			{
				return false;
			}
			l_prf_size = l_prf_size - 1;

			double l_tot_sum = 0.0;
			for (size_t i = 0; i < l_prf_size; ++i)
			{
				size_t j = (i + 1) % l_prf_size;
				l_tot_sum += f_prf[i].m_x * f_prf[j].m_y - f_prf[j].m_x * f_prf[i].m_y;
			}
			return l_tot_sum < 0.0;
		}

		/**
		* @brief Adds an edge portion to a grid cell.
		*
		* Computes slope, inverse slope, bounding box and displacement of
		* the segment from (@p f_xa, @p f_ya) to (@p f_xb, @p f_yb) and appends
		* the resulting record to the cell. Horizontal and vertical portions
		* get HUGE as slope or inverse slope respectively.
		*
		* @param f_grid_cell_ptr Cell that receives the record.
		* @param f_xa            X-coordinate of the portion start point.
		* @param f_ya            Y-coordinate of the portion start point.
		* @param f_xb            X-coordinate of the portion end point.
		* @param f_yb            Y-coordinate of the portion end point.
		* @param f_eps           Tolerance used to detect degenerate portions.
		*
		* @return false if the portion has (nearly) zero length, which means
		*         the edge crossed a grid corner and the grid must be rebuilt;
		*         true otherwise.
		*/
		inline bool AddEdgeToGrid(TGridCell* f_grid_cell_ptr,
								  const double f_xa,
								  const double f_ya,
								  const double f_xb,
								  const double f_yb,
								  const double f_eps) const
		{
			double l_slope = 0.0;
			double l_inv_slope = 0.0;

			if (EqualWithingTol(f_ya, f_yb, f_eps))
			{
				if (EqualWithingTol(f_xa, f_xb, f_eps))
				{
					return false;
				}
				else
				{
					l_slope = HUGE;
					l_inv_slope = 0.0;
				}
			}
			else
			{
				if (EqualWithingTol(f_xa, f_xb, f_eps))
				{
					l_slope = 0.0;
					l_inv_slope = HUGE;
				}
				else
				{
					l_slope = (f_xb - f_xa) / (f_yb - f_ya);
					l_inv_slope = (f_yb - f_ya) / (f_xb - f_xa);
				}
			}

			f_grid_cell_ptr->m_grid_cell_records.emplace_back(TEdgeCellRecord());
			TEdgeCellRecord& l_edge_rec = f_grid_cell_ptr->m_grid_cell_records.back();

			l_edge_rec.m_slope = l_slope;
			l_edge_rec.m_slope_inv = l_inv_slope;

			l_edge_rec.m_xa = f_xa;
			l_edge_rec.m_ya = f_ya;
			if (f_xa <= f_xb)
			{
				l_edge_rec.m_min_x = f_xa;
				l_edge_rec.m_max_x = f_xb;
			}
			else
			{
				l_edge_rec.m_min_x = f_xb;
				l_edge_rec.m_max_x = f_xa;
			}
			if (f_ya <= f_yb)
			{
				l_edge_rec.m_min_y = f_ya;
				l_edge_rec.m_max_y = f_yb;
			}
			else
			{
				l_edge_rec.m_min_y = f_yb;
				l_edge_rec.m_max_y = f_ya;
			}

			l_edge_rec.m_ax = f_xb - f_xa;
			l_edge_rec.m_ay = f_yb - f_ya;

			return true;
		}

		/**
		* @brief Checks whether two values are within a specified tolerance
		*
		* Returns true if the absolute difference between @p a and @p b
		* is strictly less than @p eps.
		*
		* @param f_a	First value to compare.
		* @param f_b	Second value to compare.
		* @param f_eps	Tolerance used for the comparison; should be non-negative.
		*
		* @return true if |f_a - f_b| < f_eps, false otherwise.
		*/
		inline bool EqualWithingTol(const double f_a, const double f_b, const double f_eps) const
		{
			return std::abs(f_a - f_b) < f_eps;
		}

		/**
		* @brief Checks whether a point lies on a line segment within a tolerance.
		*
		* @param f_px	X-coordinate of the query point.
		* @param f_py	Y-coordinate of the query point.
		* @param f_x1	X-coordinate of the first endpoint.
		* @param f_y1	Y-coordinate of the first endpoint.
		* @param f_x2	X-coordinate of the second endpoint.
		* @param f_y2	Y-coordinate of the second endpoint.
		* @param f_eps	Tolerance for the collinearity and segment-bound checks.
		* @return true if the point lies on the segment within the tolerance.
		*/
		inline bool PointOnSegment(double f_px, double f_py,
								   double f_x1, double f_y1,
								   double f_x2, double f_y2,
								   double f_eps) const
		{
			const double l_seg_dx = f_x2 - f_x1;
			const double l_seg_dy = f_y2 - f_y1;

			const double l_cross = (f_px - f_x1) * l_seg_dy - (f_py - f_y1) * l_seg_dx;
			const double l_seg_length = std::sqrt(l_seg_dx * l_seg_dx + l_seg_dy * l_seg_dy);

			if (std::abs(l_cross) > f_eps * l_seg_length)
			{
				return false;
			}

			return f_px >= std::min(f_x1, f_x2) - f_eps &&
				f_px <= std::max(f_x1, f_x2) + f_eps &&
				f_py >= std::min(f_y1, f_y2) - f_eps &&
				f_py <= std::max(f_y1, f_y2) + f_eps;
		}

		/**
		* @brief Calculates the grid cell indices containing a given point
		*
		* Converts the point's X and Y coordinates into grid cell indices
		* using the grid's minimum coordinates and inverse cell dimensions.
		*
		* @param	  f_grid			Reference to the grid structure.
		* @param	  f_pt_x			X-coordinate of the point.
		* @param	  f_pt_y			Y-coordinate of the point.
		* @param[out] f_grid_cell_id_x	X-index of the grid cell containing the point.
		* @param[out] f_grid_cell_id_y	Y-index of the grid cell containing the point.
		*
		* @note The output indices are assigned integer values converted to double.
		*       The point is assumed to lie within the grid bounds.
		*/
		void GetGridCellId(const TGrid& f_grid,
						   const double f_pt_x,
						   const double f_pt_y,
						   int& f_grid_cell_id_x,
						   int& f_grid_cell_id_y) const
		{
			f_grid_cell_id_x = (int)((f_pt_x - f_grid.m_min_x) * f_grid.m_cell_x_width_inv);
			f_grid_cell_id_y = (int)((f_pt_y - f_grid.m_min_y) * f_grid.m_cell_y_width_inv);
		}

		/**
		* @brief Builds the acceleration grid for a single polygon.
		*
		* Strategy:
		* - Get the bounds of the polygon and allocate the grid.
		* - "Walk" each edge of the polygon, noting which cell borders are
		*   crossed and which cells are entered (points on a grid line are
		*   always considered to be above that line), and store in each cell
		*   the portions of edges overlapping it.
		* - If an edge passes exactly through a grid corner, shift the grid
		*   slightly and start again.
		* - Use the parity of the horizontal cell borders to determine
		*   whether each grid corner is inside or outside.
		* - For each cell, choose a border with no edges crossing it to be
		*   used for shooting test rays (or the corner if none is clear).
		*
		* @param f_polygon  Polygon vertices. The polygon is implicitly
		*                   closed: the last point must NOT repeat the first.
		* @param f_grid_res Number of cells along X and along Y.
		* @param[out] f_grid Grid to be filled.
		*
		* @return false if the polygon has fewer than 3 vertices or
		*         @p f_grid_res < 1, true otherwise.
		*/
		inline bool GridSetup(const std::vector<CPoint2>& f_polygon,
							  const int f_grid_res,
							  TGrid& f_grid) const
		{
			if (f_polygon.size() < 3)
			{
				return false;
			}
			if (f_grid_res < 1)
			{
				return false;
			}
			const double l_eps = f_grid.InitializeGridBounds(f_polygon, f_grid_res);
			bool l_cell_setup_ok = false;
			while (!l_cell_setup_ok)
			{
				f_grid.ClearGrid();
				f_grid.InitializeGridLayout();


				CPoint2 l_edge_vert_0 = f_polygon.back();
				for (size_t i = 0; i < f_polygon.size(); i++)
				{
					const CPoint2& l_edge_vert_1 = f_polygon[i];
					CPoint2 l_edge_vert_a;
					CPoint2 l_edge_vert_b;
					if (l_edge_vert_0.m_y < l_edge_vert_1.m_y)
					{
						l_edge_vert_a = l_edge_vert_0;
						l_edge_vert_b = l_edge_vert_1;
					}
					else
					{
						l_edge_vert_a = l_edge_vert_1;
						l_edge_vert_b = l_edge_vert_0;
					}

					const double l_edge_x_extent = l_edge_vert_b.m_x - l_edge_vert_a.m_x;
					const double l_edge_y_extent = l_edge_vert_b.m_y - l_edge_vert_a.m_y;
					const double l_edge_len = sqrt(l_edge_x_extent * l_edge_x_extent + l_edge_y_extent * l_edge_y_extent);

					if (l_edge_len > 0.0)
					{
						const double l_x_dir = l_edge_x_extent / l_edge_len;
						const double l_y_dir = l_edge_y_extent / l_edge_len;
						int l_grid_cell_id_x = 0;
						int l_grid_cell_id_y = 0;
						GetGridCellId(f_grid,
									  l_edge_vert_a.m_x,
									  l_edge_vert_a.m_y,
									  l_grid_cell_id_x,
									  l_grid_cell_id_y);

						double l_dist_to_next_grid_edge_x = 0.0;
						double l_dist_to_next_grid_edge_y = 0.0;
						double l_dist_to_next_grid_edge_x_inv = 0.0;
						double l_dist_to_next_grid_edge_y_inv = 0.0;
						double l_dist_btw_two_grid_edege_x = 0.0;
						double l_dist_btw_two_grid_edege_y = 0.0;

						const bool l_edge_is_vertical = (l_edge_vert_a.m_x == l_edge_vert_b.m_x);
						const bool l_edge_is_horizontal = (l_edge_vert_a.m_y == l_edge_vert_b.m_y);

						int l_edge_sign_x = 0;
						if (l_edge_is_vertical)
						{

							l_edge_sign_x = 0;
							l_dist_to_next_grid_edge_x = HUGE;
							l_dist_btw_two_grid_edege_x = 0;
						}
						else
						{

							l_dist_to_next_grid_edge_x_inv = l_edge_len / l_edge_x_extent;
							l_dist_to_next_grid_edge_x = f_grid.m_cell_x_width * (double)l_grid_cell_id_x + f_grid.m_min_x - l_edge_vert_a.m_x;
							if (l_edge_vert_a.m_x < l_edge_vert_b.m_x)
							{
								l_edge_sign_x = 1;
								l_dist_to_next_grid_edge_x += f_grid.m_cell_x_width;
								l_dist_btw_two_grid_edege_x = f_grid.m_cell_x_width * l_dist_to_next_grid_edge_x_inv;
							}
							else
							{
								l_edge_sign_x = -1;
								l_dist_btw_two_grid_edege_x = -f_grid.m_cell_x_width * l_dist_to_next_grid_edge_x_inv;
							}
							l_dist_to_next_grid_edge_x *= l_dist_to_next_grid_edge_x_inv;
						}

						if (l_edge_is_horizontal)
						{

							l_dist_to_next_grid_edge_y = HUGE;
							l_dist_btw_two_grid_edege_y = 0;
						}
						else
						{

							l_dist_to_next_grid_edge_y_inv = l_edge_len / l_edge_y_extent;
							l_dist_to_next_grid_edge_y = (f_grid.m_cell_y_width * (double)(l_grid_cell_id_y + 1) + f_grid.m_min_y - l_edge_vert_a.m_y) * l_dist_to_next_grid_edge_y_inv;
							l_dist_btw_two_grid_edege_y = f_grid.m_cell_y_width * l_dist_to_next_grid_edge_y_inv;
						}


						double l_edge_rec_vert_0_x = l_edge_vert_a.m_x;
						double l_edge_rec_vert_0_y = l_edge_vert_a.m_y;

						TGridCell* l_grid_cell_ptr = &f_grid.m_cells[l_grid_cell_id_y * f_grid.m_x_res + l_grid_cell_id_x];

						double l_grid_closest_edge_dist = 0.0;
						bool l_y_flag = false;
						while (l_grid_closest_edge_dist < l_edge_len)
						{
							double l_edge_rec_vert_1_x = 0.0;
							double l_edge_rec_vert_1_y = 0.0;
							if (l_dist_to_next_grid_edge_x <= l_dist_to_next_grid_edge_y)
							{
								l_grid_cell_id_x += l_edge_sign_x;

								l_dist_to_next_grid_edge_y -= l_dist_to_next_grid_edge_x;
								l_grid_closest_edge_dist += l_dist_to_next_grid_edge_x;
								l_dist_to_next_grid_edge_x = l_dist_btw_two_grid_edege_x;

								if (l_grid_closest_edge_dist < l_edge_len)
								{
									l_edge_rec_vert_1_y = l_grid_closest_edge_dist * l_y_dir + l_edge_vert_a.m_y;
									if (l_edge_sign_x > 0)
									{
										l_grid_cell_ptr->m_gc_flags |= GC_R_EDGE_HIT;
										l_edge_rec_vert_1_x = f_grid.m_cells_x_values[l_grid_cell_id_x];
									}
									else
									{
										l_grid_cell_ptr->m_gc_flags |= GC_L_EDGE_HIT;
										l_edge_rec_vert_1_x = f_grid.m_cells_x_values[l_grid_cell_id_x + 1];
									}
								}
								else
								{
									l_edge_rec_vert_1_x = l_edge_vert_b.m_x;
									l_edge_rec_vert_1_y = l_edge_vert_b.m_y;
								}

								l_y_flag = false;

							}
							else
							{
								l_grid_cell_id_y++;

								l_dist_to_next_grid_edge_x -= l_dist_to_next_grid_edge_y;
								l_grid_closest_edge_dist += l_dist_to_next_grid_edge_y;
								l_dist_to_next_grid_edge_y = l_dist_btw_two_grid_edege_y;

								if (l_grid_closest_edge_dist < l_edge_len)
								{
									l_grid_cell_ptr->m_gc_flags |= GC_T_EDGE_HIT;
									l_grid_cell_ptr->m_gc_flags ^= GC_T_EDGE_PARITY;
									l_edge_rec_vert_1_x = l_grid_closest_edge_dist * l_x_dir + l_edge_vert_a.m_x;
									l_edge_rec_vert_1_y = f_grid.m_cells_y_values[l_grid_cell_id_y];
								}
								else
								{
									l_edge_rec_vert_1_x = l_edge_vert_b.m_x;
									l_edge_rec_vert_1_y = l_edge_vert_b.m_y;
								}

								l_y_flag = true;
							}

							/* check for corner crossing, then mark the cell we're in */
							if (!AddEdgeToGrid(l_grid_cell_ptr,
											   l_edge_rec_vert_0_x,
											   l_edge_rec_vert_0_y,
											   l_edge_rec_vert_1_x,
											   l_edge_rec_vert_1_y,
											   l_eps))
							{
								/* warning, danger - we have just crossed a corner.
								 * There are all kinds of topological messiness we could
								 * do to get around this case, but they're a headache.
								 * The simplest recovery is just to change the extents a bit
								 * and redo the meshing, so that hopefully no edges will
								 * perfectly cross a corner.  Since it's a preprocess, we
								 * don't care too much about the time to do it.
								 */

								 /* make the bounding box ever so slightly larger, hopefully
								  * changing the alignment of the corners.
								  */
								f_grid.MoveBBoxMin();

								break;
							}

							l_cell_setup_ok = true;

							if (l_grid_closest_edge_dist < l_edge_len)
							{
								l_grid_cell_ptr = &f_grid.m_cells[l_grid_cell_id_y * f_grid.m_x_res + l_grid_cell_id_x];
								if (l_y_flag)
								{
									l_grid_cell_ptr->m_gc_flags |= GC_B_EDGE_HIT;
									l_grid_cell_ptr->m_gc_flags ^= GC_B_EDGE_PARITY;
								}
								else
								{
									if (l_edge_sign_x > 0)
									{
										l_grid_cell_ptr->m_gc_flags |= GC_L_EDGE_HIT;
									}
									else
									{
										l_grid_cell_ptr->m_gc_flags |= GC_R_EDGE_HIT;
									}
								}
							}

							l_edge_rec_vert_0_x = l_edge_rec_vert_1_x;
							l_edge_rec_vert_0_y = l_edge_rec_vert_1_y;
						}
					}

					if (!l_cell_setup_ok)
					{
						break;
					}

					l_edge_vert_0 = l_edge_vert_1;
				}
			}

			for (int i = 1; i < f_grid.m_y_res; i++)
			{
				bool l_io_state = false;

				for (int j = 0; j < f_grid.m_x_res; j++)
				{
					TGridCell& l_grid_cell = f_grid.m_cells[(i - 1) * f_grid.m_x_res + j];
					TGridCell& l_near_grid_cell = f_grid.m_cells[i * f_grid.m_x_res + j];

					if (l_io_state)
					{
						l_grid_cell.m_gc_flags |= GC_TL_IN;
						l_near_grid_cell.m_gc_flags |= GC_BL_IN;
					}

					if (l_grid_cell.m_gc_flags & GC_T_EDGE_PARITY)
					{
						l_io_state = !l_io_state;
					}

					if (l_io_state)
					{
						l_grid_cell.m_gc_flags |= GC_TR_IN;
						l_near_grid_cell.m_gc_flags |= GC_BR_IN;
					}
				}
			}

			for (auto& l_grid_cell : f_grid.m_cells)
			{
				int l_gc_clear_flags = l_grid_cell.m_gc_flags ^ GC_ALL_EDGE_CLEAR;
				if (l_gc_clear_flags & GC_L_EDGE_CLEAR)
				{
					l_grid_cell.m_gc_flags |= GC_AIM_L;
				}
				else
				{
					if (l_gc_clear_flags & GC_B_EDGE_CLEAR)
					{
						l_grid_cell.m_gc_flags |= GC_AIM_B;
					}
					else
					{
						if (l_gc_clear_flags & GC_R_EDGE_CLEAR)
						{
							l_grid_cell.m_gc_flags |= GC_AIM_R;
						}
						else
						{
							if (l_gc_clear_flags & GC_T_EDGE_CLEAR)
							{
								l_grid_cell.m_gc_flags |= GC_AIM_T;
							}
							else
							{
								l_grid_cell.m_gc_flags |= GC_AIM_C;
							}
						}
					}
				}
			}

			return true;

		}

		/**
		* @brief Classifies a point against a single polygon grid.
		*
		* Algorithm:
		* - Check the bounding box; if the point is outside, return.
		* - Find the cell containing the point. If the cell has no edges,
		*   its state is the state of its corners.
		* - Otherwise, check whether the point lies on one of the cell's
		*   edge portions.
		* - If not, shoot a test ray towards the cell border (or corner)
		*   chosen during setup, count the crossings and combine them with
		*   the known state of that border or corner.
		*
		* @param f_grid Grid built by GridSetup().
		* @param f_x    X-coordinate of the query point.
		* @param f_y    Y-coordinate of the query point.
		*
		* @return 0 if the point is outside,
		*         1 if the point is inside,
		*         2 if the point lies on one of the polygon edges.
		*/
		inline int GridTest(const TGrid& f_grid, const double f_x, const double f_y) const
		{
			bool l_inside_flag = false;
			bool l_init_flag = false;

			if (f_y < f_grid.m_min_y || f_y >= f_grid.m_max_y || f_x < f_grid.m_min_x || f_x >= f_grid.m_max_x)
			{

				l_inside_flag = false;
			}
			else
			{

				int l_xcell = 0;
				int l_ycell = 0;
				GetGridCellId(f_grid, f_x, f_y, l_xcell, l_ycell);
				const TGridCell& l_grid_cell = f_grid.m_cells[l_ycell * f_grid.m_x_res + l_xcell];

				if (l_grid_cell.m_grid_cell_records.size() > 0)
				{

					unsigned short l_gc_flags = l_grid_cell.m_gc_flags;
					const std::vector<TEdgeCellRecord>& l_cell_records = l_grid_cell.m_grid_cell_records;

					for (size_t j = 0; j < l_cell_records.size(); ++j)
					{
						const TEdgeCellRecord& l_edge_record = l_cell_records[j];
						double l_start_x = 0.0;
						double l_start_y = 0.0;
						double l_end_x = 0.0;
						double l_end_y = 0.0;
						l_edge_record.GetRecordEndpoints(l_start_x, l_start_y, l_end_x, l_end_y);
						if (PointOnSegment(f_x, f_y, l_start_x, l_start_y, l_end_x, l_end_y, EPSILON_COINC))
						{
							return 2;
						}
					}

					switch (l_gc_flags & GC_AIM)
					{
					case GC_AIM_L:
						l_inside_flag = l_gc_flags & GC_BL_IN;
						for (size_t j = 0; j < l_cell_records.size(); ++j)
						{
							const TEdgeCellRecord& l_edge_rec_j = l_cell_records[j];

							if (f_y >= l_edge_rec_j.m_min_y && f_y < l_edge_rec_j.m_max_y)
							{
								if (f_x > l_edge_rec_j.m_max_x)
								{
									l_inside_flag = !l_inside_flag;
								}
								else if (f_x > l_edge_rec_j.m_min_x)
								{
									if ((l_edge_rec_j.m_xa - (l_edge_rec_j.m_ya - f_y) * l_edge_rec_j.m_slope) < f_x)
									{
										l_inside_flag = !l_inside_flag;
									}
								}
							}
						}
						break;

					case GC_AIM_B:
						l_inside_flag = l_gc_flags & GC_BL_IN;
						for (size_t j = 0; j < l_cell_records.size(); ++j)
						{
							const TEdgeCellRecord& l_edge_rec_j = l_cell_records[j];

							if (f_x >= l_edge_rec_j.m_min_x && f_x < l_edge_rec_j.m_max_x)
							{
								if (f_y > l_edge_rec_j.m_max_y)
								{
									l_inside_flag = !l_inside_flag;
								}
								else if (f_y > l_edge_rec_j.m_min_y)
								{
									if ((l_edge_rec_j.m_ya - (l_edge_rec_j.m_xa - f_x) * l_edge_rec_j.m_slope_inv) < f_y)
									{
										l_inside_flag = !l_inside_flag;
									}
								}
							}
						}
						break;

					case GC_AIM_R:
						l_inside_flag = (l_gc_flags & GC_TR_IN) ? 1 : 0;

						for (size_t j = 0; j < l_cell_records.size(); ++j)
						{
							const TEdgeCellRecord& l_edge_rec_j = l_cell_records[j];
							if (f_y >= l_edge_rec_j.m_min_y && f_y < l_edge_rec_j.m_max_y)
							{
								if (f_x <= l_edge_rec_j.m_min_x)
								{
									l_inside_flag = !l_inside_flag;
								}
								else if (f_x <= l_edge_rec_j.m_max_x)
								{
									if ((l_edge_rec_j.m_xa - (l_edge_rec_j.m_ya - f_y) * l_edge_rec_j.m_slope) >= f_x)
									{
										l_inside_flag = !l_inside_flag;
									}
								}
							}
						}
						break;

					case GC_AIM_T:
						l_inside_flag = (l_gc_flags & GC_TR_IN) ? 1 : 0;
						for (size_t j = 0; j < l_cell_records.size(); ++j)
						{
							const TEdgeCellRecord& l_edge_rec_j = l_cell_records[j];
							if (f_x >= l_edge_rec_j.m_min_x && f_x < l_edge_rec_j.m_max_x)
							{
								if (f_y <= l_edge_rec_j.m_min_y)
								{
									l_inside_flag = !l_inside_flag;
								}
								else if (f_y <= l_edge_rec_j.m_max_y)
								{
									if ((l_edge_rec_j.m_ya - (l_edge_rec_j.m_xa - f_x) * l_edge_rec_j.m_slope_inv) >= f_y)
									{
										l_inside_flag = !l_inside_flag;
									}
								}
							}
						}
						break;

					case GC_AIM_C:
						l_inside_flag = ((l_gc_flags & GC_BL_IN) == GC_BL_IN);
						l_init_flag = true;

						const double l_corner_x = f_grid.m_cells_x_values[l_xcell];
						const double l_corner_y = f_grid.m_cells_y_values[l_ycell];
						double l_bx = 0.0;
						double l_by = 0.0;

						for (size_t j = 0; j < l_cell_records.size(); ++j)
						{
							const TEdgeCellRecord& l_edge_rec_j = l_cell_records[j];

							if (f_x >= l_edge_rec_j.m_min_x && f_y >= l_edge_rec_j.m_min_y)
							{
								if (l_init_flag)
								{
									l_bx = f_x - l_corner_x;
									l_by = f_y - l_corner_y;
									l_init_flag = false;
								}

								const double l_denom = l_edge_rec_j.m_ay * l_bx - l_edge_rec_j.m_ax * l_by;
								if (l_denom != 0.0)
								{
									double l_cx = l_edge_rec_j.m_xa - f_x;
									double l_cy = l_edge_rec_j.m_ya - f_y;
									const double l_alpha = l_by * l_cx - l_bx * l_cy;
									if (l_denom > 0.0)
									{
										if (l_alpha < 0.0 || l_alpha >= l_denom)
										{
											continue;
										}
										const double beta = l_edge_rec_j.m_ax * l_cy - l_edge_rec_j.m_ay * l_cx;
										if (beta < 0.0 || beta >= l_denom)
										{
											continue;
										}
									}
									else
									{
										if (l_alpha > 0.0 || l_alpha <= l_denom)
										{
											continue;
										}
										const double beta = l_edge_rec_j.m_ax * l_cy - l_edge_rec_j.m_ay * l_cx;
										if (beta > 0.0 || beta <= l_denom)
										{
											continue;
										}
									}
									l_inside_flag = !l_inside_flag;
								}
							}
						}
						break;
					}
				}
				else
				{
					 l_inside_flag = l_grid_cell.m_gc_flags & GC_BL_IN;
				}
			}

			return l_inside_flag;
		}

		/** @brief Reference to the input profiles (closed, first point repeated as last). */
		std::vector<std::vector<CPoint2>>& m_input_profiles;
		/** @brief One acceleration grid per input profile, in the same order. */
		std::vector<TGrid> m_grids;
		/** @brief Number of grid cells along X and along Y. */
		const int m_resolution;
	};
}
#endif