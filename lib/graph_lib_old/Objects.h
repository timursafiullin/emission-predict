#ifndef OBJECTS_H
#define OBJECTS_H 1

#include "FL/Fl_Table.H"
#include "FL/Fl_draw.h"
#include "FL/Fl_Box.H"
#include "fltk_design.h"
#include <iostream>
#include <vector>
#include <string>

constexpr char context_column = 'C';
constexpr char context_row = 'R';


struct Labels
{
    unsigned int row{};
    unsigned int col{};
    char context{};

    Labels() {};
    Labels(std::initializer_list<std::string> labels, const char context, unsigned int line);

    std::string operator[](const unsigned int index);
    std::vector<std::string> get() const;
private:
    std::vector<std::string> labelList;
};


struct LabelsList
{
    LabelsList() {};
    LabelsList(std::initializer_list<Labels> labels);

    std::vector<Labels> get() const;
private:
    std::vector<Labels> labelsList;
};


class Table : public Fl_Table
{
public:
    Table
    (
        int x, int y, int w, int h,
        int table_cols,
        int table_rows,
        Fl_Color inner_color = FL_BLACK,
        Fl_Color outer_color = FL_BLACK,
        Fl_Color background_color = FL_WHITE
    );

    void set_label(Labels& labels);
    void set_label(LabelsList& labels_list);

private:
    int x, y, w, h;
    int rows, cols;
    int cell_w, cell_h;

    Fl_Color inner_color, outer_color, background_color;

    void draw_borders(Fl_Color color);
    void draw_borders(Fl_Color inner_color, Fl_Color outer_color);
};

template <typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;

    Node(const T& data) : data(data), next(nullptr), prev(nullptr) {}
};

template <typename T>
class dlcList {
public:
    Node<T>* head;
    Node<T>* current;
    Node<T>* tail;

    dlcList() : head(nullptr), current(nullptr), tail(nullptr) {}

    ~dlcList() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void insert(const T& data) {
        Node<T>* newNode = new Node<T>(data);

        if (head == nullptr) {
            head = newNode;
            newNode->next = newNode;
            newNode->prev = newNode;
            current = head;
            tail = head;
        } else {
            newNode->next = head;
            newNode->prev = tail;
            tail->next = newNode;
            head->prev = newNode;
            tail = newNode;
        }
    }

    T get_next() {
        if (current == nullptr) {
            throw std::runtime_error("List is empty");
        }
        current = current->next;
        return current->data;
    }

    T get_previous() {
        if (current == nullptr) {
            throw std::runtime_error("List is empty");
        }
        current = current->prev;
        return current->data;
    }

    T get_current() {
        if (current == nullptr) {
            throw std::runtime_error("List is empty");
        }
        return current->data;
    }
};

#endif // OBJECTS_H