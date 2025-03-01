// // 测试用例
// #include <gtest/gtest.h>
// #include "rat.h"

// TEST(RATTest, Initialization)
// {
//     RAT rat(5);
//     EXPECT_FALSE(rat.idleIds.empty());
// }

// TEST(RATTest, AllocateRecordId)
// {
//     RAT rat(2);
//     int id;
//     EXPECT_TRUE(rat.new_record_id(id));
//     EXPECT_EQ(id, 1);                    // 假设第一次分配最小ID
//     EXPECT_FALSE(rat.new_record_id(id)); // 空闲列表为空时无法分配
// }

// TEST(RATTest, ReleaseRecordId)
// {
//     RAT rat(2);
//     int id;
//     rat.new_record_id(id); // 分配ID 1
//     rat.delete_record_id(1);
//     EXPECT_EQ(rat.idleIds.front(), 1);
// }

// TEST(RATTest, ClearAllRecords)
// {
//     RAT rat(3);
//     int id;
//     rat.new_record_id(id); // 分配ID 1
//     rat.new_record_id(id); // 分配ID 2
//     rat.clear();
//     EXPECT_FALSE(rat.idleIds.empty());
// }

// int main(int argc, char **argv)
// {
//     testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }