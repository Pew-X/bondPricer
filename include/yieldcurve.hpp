#ifndef YIELD_CURVE_HPP
#define YIELD_CURVE_HPP

#include <type_traits>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <vector>
#include <boost/python.hpp>

namespace BondLibrary {
struct YieldCurvePoint {
    YieldCurvePoint(double maturity, double yield)
        : maturity(maturity), yield(yield) {}
    bool operator==(const YieldCurvePoint& rhs) {
        return maturity == rhs.maturity && yield == rhs.yield;
    }
    double maturity;
    double yield;
};
class YieldCurve {
public:
    YieldCurve(boost::python::list& curve_points) {
        addToCurve(curve_points);
    }
    void addToYieldCurve(boost::python::list& curve_points) {
        addToCurve(curve_points);
    }
    void removeFromYieldCurve(const boost::python::object& pt) {
        try {
            auto pt_extract = boost::python::extract<YieldCurvePoint>(pt);
            if (pt_extract.check()) {
                yield_curve_.erase(
                    std::remove(yield_curve_.begin(), yield_curve_.end(), pt), yield_curve_.end()
                );
            }
            else {
                throw std::runtime_error("Tried to remove a Yield Curve element that was not a YieldCurvePoint");
            }
        }
        catch (const boost::python::error_already_set&) {
            PyErr_Print();
        }
    }
    const std::vector<YieldCurvePoint>& getYieldCurve() const {return yield_curve_;}
private:
    void addToCurve(boost::python::list& curve_points) {
        try {
            const boost::python::ssize_t len = boost::python::len(curve_points);
            for (auto i = 0; i < len; ++i) {
                auto element = boost::python::extract<YieldCurvePoint>(curve_points[i]);
                if (element.check()) {
                    yield_curve_.push_back(element);
                }
                else {
                    throw std::runtime_error("Tried to construct a Yield Curve without YieldCurvePoints");
                }
            }
        }
        catch (const boost::python::error_already_set&) {
            PyErr_Print();
        }
    }
    std::vector<YieldCurvePoint> yield_curve_;
};
}
#endif
