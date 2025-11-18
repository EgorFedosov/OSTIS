#include <sc-memory/test/sc_test.hpp>
#include <sc-memory/sc_memory.hpp>
#include "../agent/find_intersection_agent.hpp"
#include "../agent/find_union_agent.hpp"
#include "../keynodes/partners_keynodes.hpp"

using AgentTest = ScMemoryTest;

void link_partner(ScMemoryContext * ctx, ScAddr src, ScAddr trg, ScAddr rel)
{
    ScAddr arc = ctx->GenerateConnector(ScType::ConstCommonArc, src, trg);
    ctx->GenerateConnector(ScType::ConstPermPosArc, rel, arc);
}

void init_data(ScMemoryContext * ctx, ScAddr & c1, ScAddr & c2, ScAddr & p1, ScAddr & p2, ScAddr & p3, ScAddr & p4)
{
    c1 = ctx->GenerateNode(ScType::ConstNode);
    c2 = ctx->GenerateNode(ScType::ConstNode);
    p1 = ctx->GenerateNode(ScType::ConstNode);
    p2 = ctx->GenerateNode(ScType::ConstNode);
    p3 = ctx->GenerateNode(ScType::ConstNode);
    p4 = ctx->GenerateNode(ScType::ConstNode);

    ctx->GenerateConnector(ScType::ConstPermPosArc, PartnersKeynodes::concept_company, c1);
    ctx->GenerateConnector(ScType::ConstPermPosArc, PartnersKeynodes::concept_company, c2);

    // c1: p1, p2, p3
    link_partner(ctx, c1, p1, PartnersKeynodes::nrel_partner);
    link_partner(ctx, c1, p2, PartnersKeynodes::nrel_partner);
    link_partner(ctx, c1, p3, PartnersKeynodes::nrel_partner);

    // c2: p2, p3, p4
    link_partner(ctx, c2, p2, PartnersKeynodes::nrel_partner);
    link_partner(ctx, c2, p3, PartnersKeynodes::nrel_partner);
    link_partner(ctx, c2, p4, PartnersKeynodes::nrel_partner);
}

TEST_F(AgentTest, IntersectionSuccess)
{
    ScAddr c1, c2, p1, p2, p3, p4;
    init_data(m_ctx.get(), c1, c2, p1, p2, p3, p4);
    m_ctx->SubscribeAgent<FindIntersectionAgent>(); 

    ScAddr args = m_ctx->GenerateNode(ScType::ConstNode);
    ScAddr a1 = m_ctx->GenerateConnector(ScType::ConstPermPosArc, args, c1); 
    m_ctx->GenerateConnector(ScType::ConstPermPosArc, PartnersKeynodes::rrel_1, a1);

    ScAddr a2 = m_ctx->GenerateConnector(ScType::ConstPermPosArc, args, c2);   
    m_ctx->GenerateConnector(ScType::ConstPermPosArc, PartnersKeynodes::rrel_2, a2);

    ScAction action = m_ctx->GenerateAction(PartnersKeynodes::action_find_intersection);
    action.SetArguments(args); 
    
    EXPECT_TRUE(action.InitiateAndWait()); 
    EXPECT_TRUE(action.IsFinishedSuccessfully()); 

    ScAddr res = action.GetResult(); 
    size_t count = 0;
    ScIterator3Ptr it = m_ctx->CreateIterator3(res, ScType::ConstPermPosArc, ScType::Unknown);
    while (it->Next()) count++;

    EXPECT_EQ(count, 2u); 
    EXPECT_TRUE(m_ctx->CheckConnector(res, p2, ScType::ConstPermPosArc));
    EXPECT_TRUE(m_ctx->CheckConnector(res, p3, ScType::ConstPermPosArc));
}

TEST_F(AgentTest, UnionSuccess)
{
    ScAddr c1, c2, p1, p2, p3, p4;
    init_data(m_ctx.get(), c1, c2, p1, p2, p3, p4);
    m_ctx->SubscribeAgent<FindUnionAgent>(); 

    ScAddr args = m_ctx->GenerateNode(ScType::ConstNode);
    ScAddr a1 = m_ctx->GenerateConnector(ScType::ConstPermPosArc, args, c1);
    m_ctx->GenerateConnector(ScType::ConstPermPosArc, PartnersKeynodes::rrel_1, a1);

    ScAddr a2 = m_ctx->GenerateConnector(ScType::ConstPermPosArc, args, c2);
    m_ctx->GenerateConnector(ScType::ConstPermPosArc, PartnersKeynodes::rrel_2, a2);

    ScAction action = m_ctx->GenerateAction(PartnersKeynodes::action_find_union);
    action.SetArguments(args); 
    
    EXPECT_TRUE(action.InitiateAndWait());
    EXPECT_TRUE(action.IsFinishedSuccessfully());

    ScAddr res = action.GetResult();
    size_t count = 0;
    ScIterator3Ptr it = m_ctx->CreateIterator3(res, ScType::ConstPermPosArc, ScType::Unknown);
    while (it->Next()) count++;

    EXPECT_EQ(count, 4u); 
}

TEST_F(AgentTest, InvalidArgs)
{
    m_ctx->SubscribeAgent<FindIntersectionAgent>(); 
    ScAction action = m_ctx->GenerateAction(PartnersKeynodes::action_find_intersection);
    EXPECT_TRUE(action.InitiateAndWait());
    EXPECT_TRUE(action.IsFinishedWithError()); 
}