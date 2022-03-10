#include "server.h"
#include "client.h"
#include <crypto.h>

//constructor:running server
Server::Server()
{
}

// adding a client to the server
std::shared_ptr<Client> Server::add_client(std::string id) 
{
    // iterator on map
    std::map<std::shared_ptr<Client>, double>::iterator itr; 

    for(itr = clients.begin(); itr != clients.end(); ++itr)
    {
        // adding 4 random digit to id
        if(itr->first->get_id() == id) 
        {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dist(1000, 9999);
            id += std::to_string(dist(mt));
        }

    }
    //making new client
    Client client{id, *this};
    std::shared_ptr<Client> tmp{ std::make_shared<Client>(client) };

    //adding client to clients and set init value to 5
    clients.insert(std::pair<std::shared_ptr<Client>, double>(tmp, 5)); 
    
    return tmp;

}

//access to a client's pointer via its id
std::shared_ptr<Client> Server::get_client(std::string id) const
{

    for(auto itr : clients)
    {

        if(itr.first->get_id() == id)
        {

            return itr.first;

        }

    }

    return nullptr;
}

//accsee to the value of each client via its id
double Server::get_wallet(std::string id) const
{
    //accesssing to id
    std::shared_ptr<Client> tmp{ get_client(id) };

    //mapping pointer to value
    return clients.find(tmp)->second;
}


//parsing trx into different outputs and fill them through method
bool Server::parse_trx(std::string trx, std::string &sender, std::string &receiver, double &value) 
{
    int first_{} , second_{};
    // variable for extracted value
    std::string str_value{};

    //check if there is two '-' in trx
    if(trx.find('-') != std::string::npos)
    {

        first_ = trx.find('-');
        if(trx.find('-', first_ + 1) != std::string::npos)
        {
            //checking special cases
            second_ = trx.find('-', first_ + 1);
            

            if(first_ != 0)
            {

               sender = trx.substr(0, first_); 

            }
            else
            {

                throw std::runtime_error("invalid trx format");

            }

            if(second_ - first_ != 1)
            {

                receiver = trx.substr(first_ + 1, second_ - first_ - 1);

            }
            else
            {
                
                throw std::runtime_error("invalid trx format");

            }

            if(second_ != trx.length() - 1)
            {

               str_value = trx.substr(second_ + 1, trx.length() - (second_) - 1 ); 

            }
            else
            {
                
                throw std::runtime_error("invalid trx format");

            }

            for(auto chr : str_value)
            {

                if (std::isdigit(chr) == 0 && chr != '.')
                {

                    throw std::runtime_error("invalid trx format");

                }

            }
            
            //storing value
            value = std::stod(str_value);
            return true;
        }
        else
        {

            throw std::runtime_error("invalid trx form");

        }

    }

    else
    {

        throw std::runtime_error("invalid trx form");

    }

}

//defining pending_trxs outside of Server class
std::vector<std::string> pending_trxs;
//adding trxs to pending_trxs
bool Server::add_pending_trx(std::string trx, std::string signature) const
{

    std::string sender{} , receiver{};
    double value{};
    //parsing
    if(parse_trx(trx, sender, receiver, value))
    {
        //checking validity of trx and adding
        bool authentic = crypto::verifySignature(get_client(sender)->get_publickey(), trx, signature);
        double sender_money{ clients.find(get_client(sender))->second };
        bool value_check = (sender_money >= value);

        if(value_check && authentic)
        {

            pending_trxs.push_back(trx);
            return true;

        }

    }
    return false ;

}

//mining
size_t Server::mine()
{
    
    size_t flag{}; //flag for checking of seccessfull mine
    size_t winner_nonce{};
    size_t finding_nonce{ 1 }; //flag for checking valid nonce
    std::string winner_id{};
    std::string mempool{}; 
    //final string of apended trxs
    for(auto tmp : pending_trxs)
    {

        mempool += tmp;

    }

    while(finding_nonce)
    {
        //taking nonce from every eche client
        for(auto& client : clients)
        {
            
            std::string mempool_backup{ mempool }; //backup just in case of neeeded
            size_t nonce{ client.first->generate_nonce() };
            mempool_backup += std::to_string(nonce);
            //hashing
            std::string hash{ crypto::sha256(mempool_backup) };
            //check for successfull
            if((hash.substr(0 , 10)).find("000") != std::string::npos)
            {

                client.second += 6.25; //reward
                winner_id = client.first->get_id();
                winner_nonce = nonce;
                flag = 1;
                finding_nonce = 0;
                break;

            }
            
        }

    }

    if(flag)
    {

        for(auto trx : pending_trxs)
        {
            
            std::string sender{};
            std::string receiver{};
            double value{};
            parse_trx(trx, sender, receiver, value);
            clients.find(get_client(sender))->second -= value;
            clients.find(get_client(receiver))->second += value;

        }

        pending_trxs.clear();//clearing pending_trxs
        std::cout << winner_id << std::endl;
        return winner_nonce;

    }
    else
    {

        return static_cast<size_t>(NULL);
        
    }

}

//showing wallets
void  show_wallets(const  Server& server)
{

 	std::cout << std::string(20, '*') << std::endl;
 	for(const auto& client: server.clients)
 	std::cout << client.first->get_id() <<  " : "  << client.second << std::endl;
 	std::cout << std::string(20, '*') << std::endl;
       
}
