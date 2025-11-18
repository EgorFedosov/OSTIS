#include "partners_module.hpp"
#include "agent/find_intersection_agent.hpp"
#include "agent/find_union_agent.hpp"

SC_MODULE_REGISTER(PartnersModule)
  ->Agent<FindIntersectionAgent>()
  ->Agent<FindUnionAgent>();