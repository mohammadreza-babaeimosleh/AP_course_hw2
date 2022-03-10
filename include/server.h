#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <string>
#include <map>
#include "client.h"
#include <crypto.h>
#include <random>
#include <vector>

//defining client for avoinding redefinition
class Client;
// telling compiler that this variable is defined somewhere in server.cpp
extern std::vector<std::string> pending_trxs;

class Server
{

public:
	Server();
	std::shared_ptr<Client> add_client(std::string id);
    std::shared_ptr<Client> get_client(std::string id) const;
    double get_wallet(std::string id) const;
	static bool parse_trx(std::string trx, std::string &sender, std::string &receiver, double &value);
	bool add_pending_trx(std::string trx, std::string signature) const;
	size_t mine();
	//accessing to private varialbe of class from out of it
	friend void  show_wallets(const  Server& server);
	
private:
	std::map<std::shared_ptr<Client>, double> clients;

};




#endif //SERVER_H

