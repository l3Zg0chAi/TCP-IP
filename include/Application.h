#ifndef __APPLICATION_H__
#define __APPLICATION_H__

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

#endif // __APPLICATION_H__