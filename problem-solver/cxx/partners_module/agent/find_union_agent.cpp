#include "find_union_agent.hpp"
#include "keynodes/partners_keynodes.hpp"
#include "utils.hpp"
#include <sc-memory/sc_memory.hpp>
#include <set>

using namespace std;

ScAddr FindUnionAgent::GetActionClass() const
{
  return PartnersKeynodes::action_find_union;
}

ScResult FindUnionAgent::DoProgram(ScAction & action)
{
  auto const & [argsAddr] = action.GetArguments<1>();

  if (!m_context.IsElement(argsAddr))
    return action.FinishWithError();

  ScAddr const company1 = resolve_arg(m_context, argsAddr, PartnersKeynodes::rrel_1);
  ScAddr const company2 = resolve_arg(m_context, argsAddr, PartnersKeynodes::rrel_2);

  if (!company1.IsValid() || !company2.IsValid())
    return action.FinishWithError();

  ScStructure result = m_context.GenerateStructure();
  set<size_t> uniqueHashes;

  auto addPartners = [&](ScAddr const & company) {
    ScIterator5Ptr it = m_context.CreateIterator5(
      company,
      ScType::ConstCommonArc,
      ScType::Unknown,
      ScType::ConstPermPosArc,
      PartnersKeynodes::nrel_partner
    );

    while (it->Next())
    {
      ScAddr const partner = it->Get(2);
      if (uniqueHashes.find(partner.Hash()) == uniqueHashes.end())
      {
          result << partner;
          uniqueHashes.insert(partner.Hash());
      }
    }
  };

  addPartners(company1);
  addPartners(company2);

  action.SetResult(result);
  return action.FinishSuccessfully();
}