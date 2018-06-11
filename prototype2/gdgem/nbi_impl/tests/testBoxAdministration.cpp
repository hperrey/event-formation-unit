//
// Created by soegaard on 6/11/18.
//

#include "../clusterer/include/NMXClustererSettings.h"
#include "../clusterer/include/NMXBoxAdministration.h"
#include "../../../test/TestBase.h"

TEST(NMXBoxAdmin_Test, BoxAdmin) {

    NMXBoxAdministration admin;

    // Check that the boxes from the stack appear as expected
    for (unsigned int i = 0; i < nmx::NBOXES; i++) {

        int boxid = admin.getBoxFromStack();

        EXPECT_EQ(boxid, nmx::NBOXES - 1 - i);

        admin.insertBoxInQueue(boxid);
    }
    // The stack is now empty

    // Test that the function returs -1, which indicates that the stack is empty.
    EXPECT_EQ(admin.getBoxFromStack(), -1);

    for (unsigned int i = 0; i < nmx::NBOXES; i++) {

        int boxid = (i + nmx::NBOXES/2)%nmx::NBOXES;

        std::cout << boxid << std::endl;

        admin.releaseBox(boxid);
    }

    // Test that the first box we get is lastbox inserted
    EXPECT_EQ(admin.getBoxFromStack(), nmx::NBOXES/2 - 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}