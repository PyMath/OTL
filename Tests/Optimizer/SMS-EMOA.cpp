/*!
Copyright (C) 2014, 申瑞珉 (Ruimin Shen)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cassert>
#include <random>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/constants/constants.hpp>
#include <OTL/Crossover/Real/SBX/SimulatedBinaryCrossover.h>
#include <OTL/Problem/DTLZ/DTLZ2.h>
#include <OTL/Optimizer/SMS-EMOA/MakeHypervolume.h>
#include <OTL/Optimizer/SMS-EMOA/CoupleCouple/SMS-EMOA.h>
#include <OTL/Optimizer/SMS-EMOA/CoupleCouple/MonteCarloSMS-EMOA.h>
#include <OTL/Indicator/Hypervolume/KMP_HV.h>
#include <OTL/Initial/Real/Uniform.h>
#include <OTL/Mutation/Real/PM/PolynomialMutation.h>

namespace sms_emoa
{
BOOST_AUTO_TEST_CASE(SMS_EMOA)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::dtlz::DTLZ2<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::real::sbx::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::real::pm::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::indicator::hypervolume::KMP_HV<_TReal> _THypervolume;
	typedef otl::optimizer::sms_emoa::MakeHypervolume<_THypervolume> _TMakeHypervolume;
	typedef otl::optimizer::sms_emoa::couple_couple::SMS_EMOA<_TReal, _TDecision, _TRandom &, _TMakeHypervolume> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem(nObjectives);
	const std::vector<_TDecision> initial = otl::initial::real::BatchUniform(random, problem.GetBoundary(), populationSize);
	_TCrossover crossover(random, 1, problem.GetBoundary(), 20);
	_TMutation mutation(random, 1 / (_TReal)problem.GetBoundary().size(), problem.GetBoundary(), 20);
	_TOptimizer optimizer(random, problem, initial, crossover, mutation, _TMakeHypervolume());
	BOOST_CHECK(problem.GetNumberOfEvaluations() == populationSize);
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
	{
		optimizer();
		BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), initial.size() + 2 * generation);
	}
}

BOOST_AUTO_TEST_CASE(MonteCarloSMS_EMOA)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::dtlz::DTLZ2<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::real::sbx::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::real::pm::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::sms_emoa::couple_couple::MonteCarloSMS_EMOA<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem(nObjectives);
	const std::vector<_TDecision> initial = otl::initial::real::BatchUniform(random, problem.GetBoundary(), populationSize);
	_TCrossover crossover(random, 1, problem.GetBoundary(), 20);
	_TMutation mutation(random, 1 / (_TReal)problem.GetBoundary().size(), problem.GetBoundary(), 20);
	_TOptimizer optimizer(random, problem, initial, crossover, mutation, 10000);
	BOOST_CHECK(problem.GetNumberOfEvaluations() == populationSize);
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
	{
		optimizer();
		BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), initial.size() + 2 * generation);
	}
}
}
