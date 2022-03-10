#include <iostream>
#include <gtest/gtest.h>
#include "server.h"
#include "client.h"

void  show_pending_transactions();

int main(int argc, char **argv)
{
    if (true) // make false to run unit-tests
    {
        Server emrica{};
        emrica.add_client("mamad");
        emrica.add_client("mahdi");
        std::string sender{};
        std::string receiver{};
        double value{};
        emrica.get_client("mamad")->transfer_money("mahdi", 3);
        emrica.get_client("mahdi")->transfer_money("mamad", 2);
        show_pending_transactions();
        

    }   
    else
    {
        ::testing::InitGoogleTest(&argc, argv);
        std::cout << "RUNNING TESTS ..." << std::endl;
        int ret{RUN_ALL_TESTS()};
        if (!ret)
            std::cout << "<<<SUCCESS>>>" << std::endl;
        else
            std::cout << "FAILED" << std::endl;
    }
    return 0;   
}

void  show_pending_transactions()
{
    std::cout  <<  std::string(20, '*') <<  std::endl;
 	for(const  auto& trx : pending_trxs)
 	std::cout << trx <<  std::endl;
 	std::cout  <<  std::string(20, '*') <<  std::endl;
}


