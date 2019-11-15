#include "src/utec/disk/btree.h"
#include "src/utec/disk/pagemanager.h"

#include <sstream>

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

using namespace utec::disk;

int main() {
  bool trunc_file = true;
  std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("btree.index", trunc_file);
  std::cout << "PAGE_SIZE: " << PAGE_SIZE << std::endl;
  std::cout << "BTREE_ORDER: " << BTREE_ORDER << std::endl;
  btree<char, BTREE_ORDER> bt(pm);
  std::string values = "zxcnmvfjdaqpirue";
  for(auto c : values) {
    bt.insert(c,1);
  }
  bt.print_tree();
  std::ostringstream out;
  bt.print(out);
  std::sort(values.begin(), values.end());

  // EXPECT_EQ(out.str(), values.c_str());
  bool expect = out.str() == values;
  std::cout << expect << "\n";

  return 0;
}
