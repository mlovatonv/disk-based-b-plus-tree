// #include <utecdf/column/column.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <utec/disk/btree.h>
#include <utec/disk/pagemanager.h>

#include <fmt/core.h>

// PAGE_SIZE 64 bytes
#define PAGE_SIZE  1024

// Other examples:
// PAGE_SIZE 1024 bytes => 1Kb
// PAGE_SIZE 1024*1024 bytes => 1Mb

// PAGE_SIZE = 2 * sizeof(long) +  (BTREE_ORDER + 1) * sizeof(int) + (BTREE_ORDER + 2) * sizeof(long)  
// PAGE_SIZE = 2 * sizeof(long) +  (BTREE_ORDER) * sizeof(int) + sizeof(int) + (BTREE_ORDER) * sizeof(long) + 2 * sizeof(long)
// PAGE_SIZE = (BTREE_ORDER) * (sizeof(int) + sizeof(long))  + 2 * sizeof(long) + sizeof(int) +  2 * sizeof(long)
//  BTREE_ORDER = PAGE_SIZE - (2 * sizeof(long) + sizeof(int) +  2 * sizeof(long)) /  (sizeof(int) + sizeof(long))

#define BTREE_ORDER   ((PAGE_SIZE - (2 * sizeof(long) + sizeof(int) +  2 * sizeof(long)) ) /  (sizeof(int) + sizeof(long)))

struct DiskBasedBtree : public ::testing::Test
{
};
using namespace utec::disk;

TEST_F(DiskBasedBtree, EasyTest) {
  bool trunc_file = true;
  std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("btree.index", trunc_file);
  btree<char, BTREE_ORDER> bt(pm);
  
  std::string values = "zxcvbnmlaskdjfhgqpwoeiruty";
  for(auto c : values) {
    bt.insert(c, 1);
  }
  
  std::ostringstream out;
  bt.print(out);

  std::string all_values = "zxcvbnmlaskdjfhgqpwoeiruty";
  std::sort(all_values.begin(), all_values.end());

  EXPECT_EQ(out.str(), all_values.c_str());
}

struct Student {
  long id;
  bool passed;
  char name[32];
  char surname[32];
  int n_credits;
};

TEST_F(DiskBasedBtree, IndexingRandomElements) {
  std::cout << "PAGE_SIZE: " << PAGE_SIZE << std::endl;
  std::cout << "BTREE_ORDER: " << BTREE_ORDER << std::endl;
 
  bool trunc_file = true;
  std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("btree.index", trunc_file);
  btree<long, BTREE_ORDER> bt(pm);

  pagemanager student_manager("students.bin");

  std::string test_filename = "random.txt";
  std::ifstream infile(test_filename);
  long n;
  long student_id = 1;
  std::vector<long> random;
  
  while (infile >> n) {
    Student s{n, false, "name", "surname", 100};
    student_manager.save(student_id, s); 
    bt.insert(n, student_id);
    student_id++;
    random.push_back(n);
  }

  infile.close();
  
  n = 123456;
  Student s{n, true, "maria", "lovaton", 102};
  student_manager.save(student_id, s); 
  bt.insert(n, student_id);
  random.push_back(n);
  
  std::ostringstream out;
  bt.print(out);

  std::ostringstream real;
  std::sort(random.begin(), random.end());
  for (int i = 0; i < random.size(); ++i) {
    real << random[i];
  }
  
  EXPECT_EQ(out.str(), real.str());
}
 
TEST_F(DiskBasedBtree, Persistence) {
  std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("btree.index");
  btree<long, BTREE_ORDER> bt(pm);

  pagemanager student_manager("students.bin");

  auto it = bt.find(123456);
  long page_id = it.get_page_id(); 
  Student s;
  student_manager.recover(page_id, s);

  std::string recover_name(s.name, 5);
  std::string real_name = "maria";

  EXPECT_EQ(recover_name, real_name);
}

