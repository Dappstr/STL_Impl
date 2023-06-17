#include <iostream>

template <typename T>
struct Node
{
    T m_value;
    Node<T>* m_next;
    Node<T>* m_prev;

    Node(T value) {
        m_value = value;
        m_next = nullptr;
        m_prev = nullptr;
    }
};

template <typename T>
class DoublyLinkedList
{
    private:
        Node<T>* m_head;
        Node<T>* m_tail;
        unsigned int m_length = 0;

    public:
        DoublyLinkedList(int value) {
            Node<T>* new_node = new Node<T>(value);
            m_head = new_node;
            m_tail = new_node;
            ++m_length;
        }

        void append(T value) {
            Node<T>* new_node = new Node<T>(value);
            if(m_length < 1) {
                m_head = new_node;
                m_tail = new_node;
            }

            else {
                m_tail->m_next = new_node;
                new_node->m_prev = m_tail;
                m_tail = new_node;
            }

            ++m_length;
        }

        void prepend(T value) {
            Node<T>* new_node = new Node<T>(value);
            if(m_length < 1) {
                m_head = new_node;
                m_tail = new_node;
            }

            else {
                new_node->m_next = m_head;
                m_head->m_prev = new_node;
                m_head = new_node;
            }

            ++m_length;
        }

        void delete_first() {
            if(m_length < 1) { return; }

            else if(m_length == 1) {
                m_head = nullptr;
                m_tail = nullptr;
            }
            
            else {
                Node<T>* temp = m_head;
                m_head = m_head->m_next;
                m_head->m_prev = nullptr;
                delete temp;
            }
        }

        void delete_last() {
            if(m_length == 0) {
                return;
            }

            else if (m_length == 1) {
                m_head = nullptr;
                m_tail = nullptr;
            }

            Node<T>* temp = m_tail;

            m_tail = m_tail->m_prev;
            m_tail->m_next = nullptr;

            delete temp;
            --m_length;
        }

        Node<T>* get(unsigned int indx) {
            if(indx < 0 || indx >= m_length) { return nullptr; }

            Node<T>* temp = m_head;
            
            if(indx < m_length/2) {
                for(int i = 0; i < indx; ++i) {
                    temp = temp->m_next;
                }
            }
            else {
                temp = m_tail;
                for(int i = m_length - 1; i > indx; --i) {
                    temp = temp->m_prev;
                }
            }

            return temp;

        }

        bool set(unsigned int indx, T value) {
            Node<T>* temp = get(indx);

            if(temp) {
               temp->m_value = value;
               return true;
            }
            return false;
        }

        bool insert(unsigned int indx, T value) {
            if(indx < 1 || indx > m_length) { return false; }

            else if(indx == 0) { prepend(value); }

            else if(indx == m_length) { append(value); }

            else {
                Node<T>* new_node = new Node<T>(value);
                Node<T>* before = get(indx - 1);
                Node<T>* after = before->m_next;

                before->m_next = new_node;
                new_node->m_prev = before;

                after->m_prev = new_node;
                new_node->m_next = after;
            }

            ++m_length;
            return true;
        }

        void delete_node(unsigned int indx) {
            if(indx < 0 || indx >= m_length) { return; }

            else if (indx == 0) { delete_first(); }

            else if(indx == m_length - 1) { delete_last(); }
        
            else {
                Node<T>* temp = get(indx);

                temp->m_prev->m_next = temp->m_next;
                temp->m_next->m_prev = temp->m_prev;

                delete temp;
            }
            -- m_length;
        }

        void swap_first_last() {
            T temp = m_head->m_value;
            m_head->m_value = m_tail->m_value;
            m_tail->m_value = temp;
        }

        bool is_palindrome() {
            Node<T>* starter_head = m_head;
            Node<T>* starter_tail = m_tail;
            for(int x = 0; x < m_length/2; ++x) {
                if(starter_head->m_value != starter_tail->m_value) {
                    return false;
                }
                starter_head=starter_head->m_next;
                starter_tail=starter_tail->m_prev;
            }
            
            return true;
        }

        void print_list() {
            Node<T>* temp = m_head;
            while(temp) {
                std::cout << temp->m_value << '\n';
                temp=temp->m_next;
            }
        }

    ~DoublyLinkedList() {
        Node<T>* temp = m_head;
        
        while(m_head) {
            m_head = m_head->m_next;
            delete temp;
            temp = m_head;
        }
    }
};
