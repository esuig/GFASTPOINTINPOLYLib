/**
* @file main.cpp
* @brief Test runner: for each folder in the cases folder, run its test and compare
*        the output with the folder's expected.txt.
*
* Usage: TESTS <cases folder> <output folder>
* Returns 0 if all tests pass, 1 otherwise. The results are also written to report.txt
* in the output folder.
*/

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../../GFASTPOINTINPOLYLib/libsrc/GFastPointInPolygonGridLib.h"

/**
* @brief Reads a list of 2D profiles from a text file.
*
* The input file must contain one section per profile. Each section starts
* with a profile header in the format "[profile N]", followed by a header
* specifying the point coordinates ("X Y Z"). Each subsequent line contains
* the X, Y and Z coordinates of one point, separated by whitespace.
* The Z coordinate is read from the file but ignored when constructing CPoint2.
* Empty lines are used to separate profiles.
*
* @param[out] f_prf_list Output list of profiles. Each profile is represented
*                        as a vector of CPoint2 points. Profiles are appended
*                        to the existing list; the list is not cleared.
* @param[in]  f_txt_filepath Full path to the input text file.
*
* @return true if the file is opened and parsed successfully; false if the
*         file cannot be opened or its contents do not match the expected
*         format.
*
* @note The file is expected to use whitespace-separated numeric coordinates.
*       The decimal separator must be compatible with the active stream locale.
*
* @par Example input file:
* @code
* [profile 0]
* X Y Z
* 1.123456789012345 2.123456789012345 0.000000000000000
* 3.123456789012345 4.123456789012345 0.000000000000000
* 5.123456789012345 6.123456789012345 0.000000000000000
*
* [profile 1]
* X Y Z
* 10.000000000000000 20.000000000000000 0.000000000000000
* 30.000000000000000 40.000000000000000 0.000000000000000
* @endcode
*
* @par Example usage:
* @code
* std::vector<std::vector<CPoint2>> l_prf_list;
*
* if (!ReadProfilesFromTxtFile(l_prf_list, L"C:\\temp\\profiles.txt"))
* {
*     // Handle file or parsing error.
* }
* @endcode
*/
static inline bool ReadProfilesFromTxtFile(std::vector<std::vector<gfastptinpolygridlib::CPoint2>>& f_prf_list,
										   const std::filesystem::path& f_file_path)
{
	std::wifstream l_file(f_file_path);

	if (!l_file.is_open())
	{
		return false;
	}

	std::wstring l_line;

	while (std::getline(l_file, l_line))
	{
		if (l_line.empty())
		{
			continue;
		}

		if (l_line.rfind(L"[profile ", 0) != 0)
		{
			return false;
		}

		if (!std::getline(l_file, l_line))
		{
			return false;
		}

		f_prf_list.emplace_back();
		auto& l_prf = f_prf_list.back();

		while (std::getline(l_file, l_line))
		{
			if (l_line.empty())
			{
				break;
			}

			if (l_line.rfind(L"[profile ", 0) == 0)
			{
				return false;
			}

			double l_x = 0.0;
			double l_y = 0.0;
			double l_z = 0.0;
			std::wistringstream l_stream(l_line);

			if (!(l_stream >> l_x >> l_y >> l_z))
			{
				return false;
			}

			l_prf.push_back({ l_x, l_y });
		}
	}

	if (l_file.bad())
	{
		return false;
	}

	return true;
}

static bool RunTest(const std::string& f_test_name,
					const std::filesystem::path& f_test_folder_path)
{
	std::vector<std::vector<gfastptinpolygridlib::CPoint2>> l_prf_list;
	if (!ReadProfilesFromTxtFile(l_prf_list, f_test_folder_path / "InputCurvesData.txt"))
	{
		throw std::runtime_error("no valid profile input file for this folder");
	}
	if (f_test_name == "simple_one_hole_in_hole")
	{
		const int l_grid_resolution = 10;
		gfastptinpolygridlib::CGFastPointInPolygonGrid l_pt_in_poly(l_prf_list, l_grid_resolution);
		int l_ret_test = l_pt_in_poly.IsPointInside(16.0, 16.0);
		if (l_ret_test != 0)
		{
			return false;
		}
	}
	if (f_test_name == "simple_one_hole_inside")
	{
		const int l_grid_resolution = 10;
		gfastptinpolygridlib::CGFastPointInPolygonGrid l_pt_in_poly(l_prf_list, l_grid_resolution);
		int l_ret_test = l_pt_in_poly.IsPointInside(10.0, 10.0);
		if (l_ret_test != 1)
		{
			return false;
		}
	}
	if (f_test_name == "simple_one_hole_outside")
	{
		const int l_grid_resolution = 10;
		gfastptinpolygridlib::CGFastPointInPolygonGrid l_pt_in_poly(l_prf_list, l_grid_resolution);
		int l_ret_test = l_pt_in_poly.IsPointInside(30.0, 35.0);
		if (l_ret_test != 0)
		{
			return false;
		}
	}
	else if (f_test_name == "simple_horizontal_vertical")
	{
		
	}
	else if (f_test_name == "simple_point_on_inner_edge")
	{

	}
	else if (f_test_name == "simple_point_on_outer_edge")
	{

	}
	else
	{
		throw std::runtime_error("no test defined for this folder");
	}

	return true;
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "usage: TESTS <cases folder> <output folder>\n";
		return 2;
	}
	const std::filesystem::path l_cases_dir_path = argv[1];
	const std::filesystem::path l_output_dir_path = argv[2];
	std::filesystem::create_directories(l_output_dir_path);
	std::ofstream l_report_file(l_output_dir_path / "report.txt");

	int l_tests_passed = 0;
	int l_test_failed = 0;
	for (const auto& l_entry : std::filesystem::directory_iterator(l_cases_dir_path))
	{
		if (!l_entry.is_directory())
		{
			continue;
		}
		const std::string l_test_name = l_entry.path().filename().string();
		const std::filesystem::path l_result_file = l_output_dir_path / (l_test_name + ".txt");

		std::string l_message;
		try
		{
			std::filesystem::remove(l_result_file);
			if (!RunTest(l_test_name, l_entry.path()))
			{
				l_message = l_test_name;
				l_message += " has failed/n";
			}
		}
		catch (const std::exception& ex)
		{
			l_message = ex.what();
		}

		const std::string l_line = (l_message.empty() ? "PASS  " : "FAIL  ") + l_test_name + (l_message.empty() ? "" : "  -> " + l_message);
		std::cout << l_line << '\n';
		l_report_file << l_line << '\n';
		l_message.empty() ? ++l_tests_passed : ++l_test_failed;
	}

	const std::string l_summary = std::to_string(l_tests_passed) + " passed, " + std::to_string(l_test_failed) + " failed";
	std::cout << l_summary << '\n';
	l_report_file << l_summary << '\n';

	return l_test_failed == 0 ? 0 : 1;
}