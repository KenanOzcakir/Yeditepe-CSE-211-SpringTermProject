#pragma once

/**
 * @brief Node used by the custom singly linked list.
 *
 * @tparam T Stored value type.
 * @pre T must be copy constructible.
 * @post next is initialized to nullptr.
 */
template <typename T>
struct ListNode {
    T value;
    ListNode<T>* next;
    ListNode(const T& v) : value(v), next(nullptr) {}
};

/**
 * @brief Pointer-based singly linked list.
 *
 * @tparam T Stored value type.
 * @pre T must be copy constructible.
 * @post Values are stored in dynamically allocated ListNode objects.
 */
template <typename T>
class LinkedList {
   public:
    /**
     * @brief Constructs an empty list.
     * @post getHead() returns nullptr.
     */
    LinkedList() : head(nullptr) {}

    /**
     * @brief Destroys the list and all nodes.
     * @post All dynamically allocated nodes are deleted.
     */
    ~LinkedList() {
        clear();
    }

    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    /**
     * @brief Returns the first node.
     *
     * @return Head node pointer, or nullptr for an empty list.
     * @post The list is not modified.
     */
    ListNode<T>* getHead() const {
        return head;
    }

    /**
     * @brief Appends a value to the end of the list.
     *
     * @param value Value to append.
     * @post The list contains value as its last element.
     */
    void append(const T& value) {
        ListNode<T>* node = new ListNode<T>(value);
        if (!head) {
            head = node;
            return;
        }
        ListNode<T>* cur = head;
        while (cur->next) {
            cur = cur->next;
        }
        cur->next = node;
    }

    /**
     * @brief Inserts a value while preserving caller-defined order.
     *
     * @tparam Compare Callable returning true when first argument ranks before second.
     * @param value Value to insert.
     * @param compare Ordering predicate.
     * @post The list remains sorted according to compare.
     */
    template <typename Compare>
    void insertSorted(const T& value, Compare compare) {
        ListNode<T>* newNode = new ListNode<T>(value);

        if (!head || compare(value, head->value)) {
            newNode->next = head;
            head = newNode;
            return;
        }

        ListNode<T>* cur = head;
        while (cur->next && compare(cur->next->value, value)) {
            cur = cur->next;
        }

        newNode->next = cur->next;
        cur->next = newNode;
    }

    /**
     * @brief Finds the first node matching a predicate.
     *
     * @tparam Predicate Callable returning true for a matching value.
     * @param pred Predicate to evaluate.
     * @return Matching node pointer, or nullptr when absent.
     * @post The list is not modified.
     */
    template <typename Predicate>
    ListNode<T>* find(Predicate pred) const {
        ListNode<T>* cur = head;
        while (cur) {
            if (pred(cur->value)) {
                return cur;
            }
            cur = cur->next;
        }
        return nullptr;
    }

    /**
     * @brief Deletes every node in the list.
     * @post getHead() returns nullptr.
     */
    void clear() {
        ListNode<T>* cur = head;
        while (cur) {
            ListNode<T>* next = cur->next;
            delete cur;
            cur = next;
        }
        head = nullptr;
    }

    /**
     * @brief Removes the final node from the list.
     * @pre The list may be empty.
     * @post The list has one fewer node when it was non-empty.
     */
    void removeLast() {
        if (!head) {
            return;
        }

        if (!head->next) {
            delete head;
            head = nullptr;
            return;
        }

        ListNode<T>* cur = head;
        while (cur->next && cur->next->next) {
            cur = cur->next;
        }

        delete cur->next;
        cur->next = nullptr;
    }

   private:
    ListNode<T>* head;
};
