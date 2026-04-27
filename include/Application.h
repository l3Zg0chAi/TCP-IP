#ifndef APPLICATION_H
#define APPLICATION_H

class Application {
public:
    static Application* get_instance(){
        static Application m_instance;
        return &m_instance;
    }

    void init();
    void execute();
    void receive_from_server();
private:

};

#endif // APPLICATION_H