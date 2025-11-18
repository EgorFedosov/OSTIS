#pragma once
#include <sc-memory/sc_keynodes.hpp>

class PartnersKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_find_intersection{
    "action_find_intersection", ScType::ConstNodeClass};
    
  static inline ScKeynode const action_find_union{
    "action_find_union", ScType::ConstNodeClass};

  static inline ScKeynode const nrel_partner{
    "nrel_partner", ScType::ConstNodeNonRole};
    
  static inline ScKeynode const rrel_1{"rrel_1", ScType::ConstNodeRole};
  static inline ScKeynode const rrel_2{"rrel_2", ScType::ConstNodeRole};

  static inline ScKeynode const concept_company{"concept_company", ScType::ConstNodeClass};
  static inline ScKeynode const concept_partner{"concept_partner", ScType::ConstNodeClass};
};