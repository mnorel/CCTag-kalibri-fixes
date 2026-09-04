/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define BOOST_TEST_MODULE testCCTagCoordinates
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <cctag/CCTag.hpp>

namespace {

using Point = cctag::Point2d<Eigen::Vector3f>;
using Ellipse = cctag::numerical::geometry::Ellipse;

cctag::CCTag makeTag(const Point& center, float scale)
{
    const Ellipse ellipse(center, 8.0f, 5.0f, 0.3f);
    const std::vector<std::vector<cctag::DirectedPoint2d<Eigen::Vector3f>>> points;
    return cctag::CCTag(-1, center, points, ellipse, Eigen::Matrix3f::Identity(),
                        0, scale);
}

} // namespace

BOOST_AUTO_TEST_SUITE(test_cctag_coordinates)

BOOST_AUTO_TEST_CASE(constructor_preserves_pixel_coordinate_system)
{
    const Point center(12.25f, -3.75f);
    const cctag::CCTag tag = makeTag(center, 1.0f);

    BOOST_CHECK_CLOSE(tag.x(), center.x(), 1e-5f);
    BOOST_CHECK_CLOSE(tag.y(), center.y(), 1e-5f);
    BOOST_CHECK_CLOSE(tag.outerEllipse().center().x(), center.x(), 1e-5f);
    BOOST_CHECK_CLOSE(tag.outerEllipse().center().y(), center.y(), 1e-5f);
    BOOST_CHECK_CLOSE(tag.rescaledOuterEllipse().center().x(), center.x(), 1e-5f);
    BOOST_CHECK_CLOSE(tag.rescaledOuterEllipse().center().y(), center.y(), 1e-5f);
}

BOOST_AUTO_TEST_CASE(rescaled_ellipse_uses_only_the_requested_scale)
{
    const Point center(12.25f, -3.75f);
    const float scale = 2.0f;
    const cctag::CCTag tag = makeTag(center, scale);

    BOOST_CHECK_CLOSE(tag.rescaledOuterEllipse().center().x(), center.x() * scale,
                      1e-5f);
    BOOST_CHECK_CLOSE(tag.rescaledOuterEllipse().center().y(), center.y() * scale,
                      1e-5f);
    BOOST_CHECK_CLOSE(tag.rescaledOuterEllipse().a(), 8.0f * scale, 1e-5f);
    BOOST_CHECK_CLOSE(tag.rescaledOuterEllipse().b(), 5.0f * scale, 1e-5f);
}

BOOST_AUTO_TEST_SUITE_END()
