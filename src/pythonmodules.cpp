#include <boost/python.hpp>
#include "yieldcurve.hpp"
#include "flattermbond.hpp"
#include "date.hpp"
#include "generaltermbond.hpp"

using namespace boost::python;
using Date = BondLibrary::Date;
using DC = BondLibrary::DayCountConvention;

struct BaseBondWrapper : ::BondLibrary::BaseBond, wrapper<BondLibrary::BaseBond> {
    BaseBondWrapper(const double f, const double c, const Date md, 
     const Date id, const BondLibrary::CashFlowsPy& cfs, const Date sd,
     const DC dcv)
        : ::BondLibrary::BaseBond(f, c, md, id, cfs, sd, dcv)
    {}
    double duration(const double rate, const Date date) const {
        return this->get_override("duration")(rate, date);
    }
};

BOOST_PYTHON_MODULE(BondPricing) {
    class_<BondLibrary::YieldCurvePoint>("YieldCurvePoint", init<double, double>((arg("maturity"), arg("bond_yield"))))
        .def_readwrite("maturity", &BondLibrary::YieldCurvePoint::maturity)
        .def_readwrite("bond_yield", &BondLibrary::YieldCurvePoint::yield)
        .def("__eq__", &BondLibrary::YieldCurvePoint::operator==);
    class_<BondLibrary::YieldCurve>("YieldCurve", init<list&>())
        .def("addToYieldCurve", &BondLibrary::YieldCurve::addToYieldCurve)
        .def("removeFromYieldCurve", &BondLibrary::YieldCurve::removeFromYieldCurve);
    class_<Date>("Date", init<const std::string&>());
    enum_<BondLibrary::DayCountConvention>("DayCountConvention")
        .value("Year360Month30", DC::Year360Month30)
        .value("Year365Month30", DC::Year365Month30)
        .value("Year360MonthActual", DC::Year360MonthActual)
        .value("Year365MonthActual", DC::Year365MonthActual)
        .value("YearActualMonthActual", DC::YearActualMonthActual);
    class_<BondLibrary::CashFlow>("CashFlow", init<double, Date>((arg("cashflow"), arg("due_date"))));
    class_<BaseBondWrapper, boost::noncopyable>("BaseBond", init<double, double, Date, Date, list, Date, DC>())
        .def("duration", pure_virtual(&BondLibrary::BaseBond::duration))
        .def("notionalPresentValue", pure_virtual(&BondLibrary::BaseBond::notionalPresentValue));
    class_<BondLibrary::FlatTermBond, bases<BaseBondWrapper>>(
        "FlatTermBond", init<double, double, Date, Date, list, Date, DC>((
            arg("face_value"), arg("coupon"), arg("maturity_date"), arg("issue_date"), 
            arg("cashflows"), arg("settlement_date")=BondLibrary::getCurrentDate() + 2, 
            arg("dc_convention")=DC::YearActualMonthActual
        )))
        .def("cleanPrice", &BondLibrary::FlatTermBond::cleanPrice)
        .def("dirtyPrice", &BondLibrary::FlatTermBond::dirtyPrice, (arg("rate"), arg("date")))
        .def("dirtyPriceFromCleanPrice", &BondLibrary::FlatTermBond::dirtyPriceFromCleanPrice)
        .def("duration", &BondLibrary::FlatTermBond::duration)
        .def("isExpired", &BondLibrary::BaseBond::isExpired)
        .def("yieldToMaturity", &BondLibrary::BaseBond::yieldToMaturity);
    class_<BondLibrary::GeneralTermBond, bases<BaseBondWrapper>>(
        "GeneralTermBond", init<double, double, Date, Date, list&, Date, BondLibrary::YieldCurve&, DC>((
            arg("face_value"), arg("coupon"), arg("maturity_date"), arg("issue_date"),
            arg("cashflows"), arg("settlement_date")=BondLibrary::getCurrentDate() + 2, 
            arg("yield_curve"), arg("dc_convention")=DC::YearActualMonthActual
        )))
        .def("cleanPrice", &BondLibrary::GeneralTermBond::cleanPrice)
        .def("dirtyPrice", &BondLibrary::GeneralTermBond::dirtyPrice)
        .def("getDuration", &BondLibrary::GeneralTermBond::getDuration)
        .def("setYieldCurve", &BondLibrary::GeneralTermBond::setYieldCurve)
        .def("isExpired", &BondLibrary::BaseBond::isExpired)
        .def("yieldToMaturity", &BondLibrary::BaseBond::yieldToMaturity);
}
