#include "find_intersection_agent.hpp"
#include "keynodes/partners_keynodes.hpp"
#include "utils.hpp" 
#include <sc-memory/sc_memory.hpp>

using namespace std;

ScAddr FindIntersectionAgent::GetActionClass() const
{
  return PartnersKeynodes::action_find_intersection;
}

ScResult FindIntersectionAgent::DoProgram(ScAction & action)
{
  auto const & [argsAddr] = action.GetArguments<1>();

  if (!m_context.IsElement(argsAddr))
    return action.FinishWithError();

  ScAddr const company1 = resolve_arg(m_context, argsAddr, PartnersKeynodes::rrel_1);
  ScAddr const company2 = resolve_arg(m_context, argsAddr, PartnersKeynodes::rrel_2);

  if (!company1.IsValid() || !company2.IsValid())
    return action.FinishWithError();

  ScStructure result = m_context.GenerateStructure();
  
  ScIterator5Ptr it = m_context.CreateIterator5(
    company1,
    ScType::ConstCommonArc,
    ScType::Unknown,
    ScType::ConstPermPosArc,
    PartnersKeynodes::nrel_partner
  );

  while (it->Next())
  {
    ScAddr const partner = it->Get(2);

    ScIterator5Ptr check = m_context.CreateIterator5(
        company2,
        ScType::ConstCommonArc,
        partner,
        ScType::ConstPermPosArc,
        PartnersKeynodes::nrel_partner
    );

    if (check->Next())
    {
      result << partner;
    }
  }

  action.SetResult(result);
  return action.FinishSuccessfully();
}