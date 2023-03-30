/*
    Чат на основе контейнеров библиотеки STL
*/

#include "chat.h"

    Chat::Chat():
        _registeredUsers(map<string, std::unique_ptr<uint[]>>()),
        _loginedUsers(set<string>()),
        _storedMessages(set<std::unique_ptr<Message>>())
    {
        //cout << "Chat created" << endl;
    }

    Chat::~Chat(){
        //cout << "Chat deleted" << endl;
    }

// регистрация и вход
    void Chat::registerUser(string name, string login){
        map<string, std::unique_ptr<uint[]>>::iterator it = _registeredUsers.find(name);

        if(it == _registeredUsers.end()) {          // if not exist
            _registeredUsers[name] = std::unique_ptr<uint[]>(sha1(login.data(), login.size()));
            _loginedUsers.insert(name);

            // std::unique_ptr<uint[]>& data = _registeredUsers[name];
            // cout << "\n" << name << "  ";
            // for(int i = 0; i < 5; ++i) cout << data[i] << " ";

            mainLoop(name);
            _loginedUsers.erase(name);
        }
        else {
            cout << "Name is occupied" << endl;
        }
    }

// логин и вход
    void Chat::loginUser(string name, string login){
        auto itset = _loginedUsers.find(name);
        if(itset != _loginedUsers.end()) {          // if exist
            cout << "User already in the chat" << endl;
            return;
        }

        map<string, std::unique_ptr<uint[]>>::iterator it = _registeredUsers.find(name);
        std::unique_ptr<uint[]> hash(sha1(login.data(), login.size()));

        // for(int i = 0; i < 5; ++i) cout << hash[i] << " ";
        // cout << endl;
        // for(int i = 0; i < 5; ++i) cout << (*it).second[i] << " ";
        // cout << endl;

        if((*it).first == name && !memcmp(&(*it).second[0], hash.get(), 20))  // memcmp return 0 if equal
        {
            _loginedUsers.insert(name);
            mainLoop(name);
            _loginedUsers.erase(name);
        }
        else {
            cout << "Wrong user " << endl;
        }
    }

// ввод строки
    void Chat::input(string& str, int len){
        while(1){            
            getline(cin, str);
            if (str.size() > len || len == 0){
                cout << "Не больше " << len << " символов" << endl;
                continue;
            }
            else break;
        }
    }

// цикл чата
    void Chat::mainLoop(string& name){
        string buf = "";
        cout << name << " вошёл в чат. Напечатайте \"exit\" для выхода\n";
        for(auto& it: _storedMessages) {
            if((*it)._to == "")            // сообщения всем в чате
            {
                cout << (*it)._from << "\t" << (*it)._message << endl;
            }
        }

        cout << "Личные сообщения\n";
        for(auto& it: _storedMessages) {
            if((*it)._to == name){
                cout << (*it)._from << " " << (*it)._message << endl;
            }
        }

        cout << "Формат сообщения:\n\tЛичное: <кому><пробел><сообщение>\n\tВсем: <сообщение>" << endl;
        cout << "Напечатайте \"exit\" для выхода\n";
        while(1){
            cout << "< " << name << " >:\t";
            getline(cin, buf);
            if(buf == "exit") break;
            _storedMessages.insert(unique_ptr<Message>(fillMessage(name, buf)));
        }
    }

// заполняем struct Chat::Message
    Chat::Message* Chat::fillMessage(string& name, string& buf){
        string bufer = "";
        Message* m = new Chat::Message();
        size_t i = 0;
        for(; i < buf.size(); ++i){
            if(buf[i] != ' ') bufer += buf[i];
            else break;
        }

        cout << "\n" << "bufer " << bufer << endl;

        map<string, unique_ptr<uint[]>>::iterator it = _registeredUsers.find(bufer);
        if(it != _registeredUsers.end()) {          // if exist
            m->_to = bufer;
            m->_message = buf.substr(m->_to.size() + 1, buf.size());
        } else {
            m->_message = buf;
        }
        m->_from = name;
        return m;
    }

// вход в чат
    void Chat::enter(){
        string userName = "";
        string userPass = "";
        int len = 10;
        char val;
        cout << "\tChat STL" << endl;

        while(true){
            cout << "1 Login\n2 Register\n3 Exit\nPress 1, 2 or 3\n";
            getline(cin, userName);

            if(userName.size() > 1) continue;
            val = userName[0];
            if(val == '1' || val =='2' || val == '3'){

                switch(val){
                    case '1':
                        cout << "Введите имя\n";
                        input(userName, len);
                        cout << "Введите пароль\n";
                        input(userPass, len);
                        loginUser(userName, userPass);
                        break;
                    case '2':
                        cout << "Введите имя\n";
                        input(userName, len);
                        cout << "Введите пароль\n";
                        input(userPass, len);
                        registerUser(userName, userPass);
                        break;
                    case '3':
                        return;
                        break;
                    default:
                        break;
                }

            }
        }
    }
