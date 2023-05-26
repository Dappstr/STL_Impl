#include <iostream>
#include <type_traits>

template <typename T>
struct Node
{
    int m_value;
    Node<T> m_next;

    Node<T>(int value) {
        m_value = value;
        m_next = nullptr;
    }
};

template <typename T>
class Linked_List
{
    private:
        Node<T>* m_head;
        Node<T>* m_tail;
        unsigned int m_length;

    public:
        Linked_List<T>(int value) {
            Node<T> node = new Node<T>(value);
            m_head = node;
            m_tail = node;
            ++m_length;
        }

        void append(int value) {
            Node<T> new_node = Node<T>(value);

            if(m_length == 0) {
                m_head = new_node;
                m_tail = new_node;
                ++m_length;
            }
            else {
                m_tail = new_node;
                m_tail->m_next = new_node;
                ++m_length;
            }
        }

        void print_list() {
            Node<T>* temp = m_head;
            while(temp) {
                std::cout << temp->m_value << ' ';
                temp=temp->m_next;
            }
        }

        ~Linked_List<T>() {
            Node<T>* place_holder = m_head;
            while(place_holder) {
                delete place_holder;
                place_holder = m_head->m_next;
            }
        }
};
