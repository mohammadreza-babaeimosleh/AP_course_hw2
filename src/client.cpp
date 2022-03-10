#include "client.h"
#include "server.h"
#include <crypto.h>

//constructor with defining 
Client::Client(std::string _id, const Server& _server)
    : server{ &_server }
    , id{ _id }

{

    crypto::generate_key(this->public_key,this->private_key);

}

//getter for id
std::string Client::get_id()
{

    return id;
    
}

//getter for publickey
std::string Client::get_publickey() const
{

    return public_key;

}

//getter for wallet
double Client::get_wallet()
{

    return server->get_wallet(id);

}

//signing trx using private key
std::string Client::sign(std::string txt) const
{

    std::string signature = crypto::signMessage(private_key, txt);
    return signature;

}

//money transfering
bool Client::transfer_money(std::string receiver, double value)
{

    std::string value_trx{ std::to_string(value) };
    if(server->get_client(receiver))//checking if receiver exists
    {

        std::string trx{ id + "-" + receiver+ "-" + value_trx };
        std::string signature{ sign(trx) };
        if(server->add_pending_trx(trx, signature))
        {

            return true;

        }
        else
        {

            return false;

        }

    }
    else
    {

        return false;

    }
    
}

//generating nonce 
size_t  Client::generate_nonce()
{
    //generating random number for nonce
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<size_t> dist;
    size_t nonce{ (dist(mt)) };
    return nonce;

}










