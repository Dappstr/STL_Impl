#pragma once

#include <iostream>

namespace dapp {
    template<typename T>
    struct Node {
        int m_value;
        Node<T>* m_next;

        Node(int value) {
            m_value = value;
            m_next = nullptr;
        }
    };

    template<typename T>
    class Linked_list {
    private:
        Node<T>* m_head;
        Node<T>* m_tail;
        int m_length;

    public:
        // Basic constructor for creating our linked list
        Linked_list(T value) {
            //Our first node is going to represent both the head and tail
            Node<T>* new_node = new Node<T>(value);
            m_head = new_node;
            m_tail = new_node;
            m_length = 1;
        }

        T get_head() { return m_head->m_value; }

        T get_tail() { return m_tail->m_value; }

        unsigned int get_length() { return m_length; }

        void print_list() {
            Node<T>* temp = m_head; //We start at the head

            //While temp is not equal to nullptr, or rather while temp points to data
            while (temp) {
                std::cout << temp->m_value << '\n';
                temp = temp->m_next;
            }
        }

        //We add a node onto the end of our linked list
        void append(T value) {
            Node<T>* new_node = new Node<T>(value);

            //If we don't already have a node.
            //This can be the case if we constructed a LL without a value, or we just destructed our LL
            if (m_length < 1) {
                m_head = new_node;
                m_tail = new_node;
            }
                //Othwerwise, simply set the end of our LL to our new node
            else {
                m_tail->m_next = new_node;
                m_tail = new_node;
            }
            ++m_length;
        }

        void prepend(T value) {
            Node<T>* new_node = new Node<T>(value);

            //Check if we don't have anything in the first place
            if (m_length == 0) {
                m_head = new_node;
                m_tail = new_node;
            }

                // Otherwise, we're going to set the next node to the *CURRENT* head
                // Then set the current head to the new node representing the new head
            else {
                new_node->m_next = m_head;
                m_head = new_node;
            }
            ++m_length;
        }

        // This function will return the node at a given index
        // It will be useful for when we want to go to a specific node within our linked list
        Node<T>* get(int indx) {
            //Error checking
            if (indx < 0 || indx >= m_length) {
                return nullptr;
            }

            Node<T>* temp = m_head;

            //Loop through our LL `indx` amount of times to get the node at the specific index
            for (int i = 0; i < indx; ++i) {
                temp = temp->m_next;
            }

            return temp;
        }

        bool set(int indx, const T &value) {
            if (indx < 0 || indx >= m_length) {
                return false;
            }

            Node<T> *temp = get(indx);

            /*This is the alternative method if we didn't have get()
            /Essentially we would iterate through until `temp` is at the given index
            Node<T>* temp = m_head;
            for(int i = 0; i < indx; ++i) {
                temp = temp->m_next;
            }
            */

            temp->m_value = value;
            return true;
        }

        bool insert(unsigned int indx, const T &value) {
            if (indx < 0 || indx >= m_length) { return false; }

                // We've already written functions for these cases
            else if (indx == 0) { prepend(value); }
            else if (indx == value) { append(value); }

            else {
                Node<T> *new_node = new Node<T>(value);
                Node<T> *temp = get(indx - 1); //Get the node previous or "behind" our new node at the index

                //Set the current new node's index to be the one ahead of it
                new_node->m_next = temp->m_next;

                //Then set the node behind our new node
                temp->m_next = new_node;
            }
            ++m_length;
            return true;
        }

        void delete_node(int indx) {
            if (indx < 0 || indx >= m_length) { return; }

                //We alredy have functions for deleting the first and last node
            else if (indx == 0) { delete_first(); }
            else if (indx == m_length - 1) { delete_last(); }

                //We get the previous element to set it to the element after the index we're deleting
            else {
                Node<T>* prev = get(indx - 1);
                Node<T>* temp = prev->m_next;

                prev->m_next = temp->m_next;

                delete temp;
                --m_length;
            }
        }

        //When deleting the first index, we're essentially just moving head up to the next node, deleting, then decrementing our length
        void delete_first() {
            if (m_length == 0) { return; }

            else if (m_length == 1) {
                m_head = nullptr;
                m_tail = nullptr;
            } else {
                Node<T>* temp = m_head;
                m_head = m_head->m_next;
                delete temp;
            }
            --m_length;
        }

        void delete_last() {
            //If our length is already 0, we don't need to delete anything
            if (m_length == 0) { return; }

            else {
                /*
                 * "temp" and "pre" will both loop through our linked list
                 * however, "temp" will consistently go ahead of "pre" to check if `next` is a nullptr
                 * if it is, this lets us know that temp signifies the end of the linked list
                 * we then set our tail to be equal to pre, decrement our length, then delete temp
                 */
                Node<T>* temp = m_head;
                Node<T>* pre = m_head;

                while (temp->m_next) {
                    pre = temp;
                    temp = temp->m_next;
                }
                m_tail = pre;
                m_tail->m_next = nullptr;
                --m_length;

                //The reason why we have this second if statement here, is if we only had 1 node, it'd be deleted leaving no nodes
                if (m_length == 0) {
                    m_head = nullptr;
                    m_tail = nullptr;
                }

                delete temp;
            }
            --m_length;
        }

        //Will find the node in the middle of the linked list
        Node<T>* find_middle_node() {

            //easy way:
            //return get(m_length / 2);

            int middle = m_length / 2;
            Node<T> *temp = m_head;

            for (int i = 0; i < middle; ++i) {
                temp = temp->m_next;
            }

            return temp;
        }


        //Find the given node which is 'k' distance from the end
        Node<T>* find_kth_from_end(int k) {
            //easy way
            //return get(m_length - k);

            Node<T> *temp = m_head;
            for (int i = 0; i < m_length - k; ++i) {
                temp = temp->m_next;
            }

            return temp;
        }

        void remove_duplicates() {
            Node<T>* temp = m_head;
            for (int x = 0; x < m_length; ++x) {
                Node<T>* cmp = temp->m_next;
                for (int y = x + 1; y < m_length; ++y) {
                    if (cmp->m_value == temp->m_value) {
                        delete_node(y);
                        cmp = temp->m_next;
                    } else {
                        cmp = cmp->m_next;
                        ++y;
                    }
                }
                temp = temp->m_next;
            }
        }

        void reverse() {
            Node<T>* temp = m_head;

            m_head = m_tail;
            m_tail = temp;

            Node<T>* after = temp->m_next;
            Node<T>* before = nullptr;

            for (int i = 0; i < m_length; ++i) {
                after = temp->m_next;
                temp->next = before;
                before = temp;
                temp = after;
            }
        }

        ~Linked_list() {
            Node<T>* temp = m_head;

            //The reason we don't use temp, is because head is consistently the start of our LL
            //We check that head is valid, if it is, it means there's still nodes to delete
            while (m_head) {
                m_head = m_head->m_next;
                delete temp;
                temp = m_head;
            }
        }
    };
}