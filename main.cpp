#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct Figure;
// Абстрактный базовый класс для всех фигур
class Figure {
protected:
    string color;  // Цвет фигуры
    string date;   // Дата создания

public:
    virtual ~Figure() {}  // Виртуальный деструктор для корректной очистки памяти
    
    // Конструктор, принимающий цвет и дату создания фигуры
    Figure(string _color, string _date)
        : color(_color), date(_date) {}

    // Абстрактный метод вывода информации о фигуре
    virtual void print() = 0;

    // Метод получения года создания фигуры из строки с датой
    int getYear() const {
        istringstream iss(date);
        int day, month, year;
        char dot;
        iss >> day >> dot >> month >> dot >> year;
        return year;
    }

    // Виртуальная функция для проверки условий (переопределяется в дочерних классах)
    virtual bool matchesCondition(const string& condition) const {
        istringstream iss(condition);
        string keyword;
        iss >> keyword;
        int year;
        iss >> year;
        if (keyword == ">") {
            return getYear() > year;
        } else if (keyword == "<") {
            return getYear() < year;
        } else if (keyword == "=") {
            return getYear() == year;
        }
        return false; // Если условие не распознано, возвращаем false
    }
};

// Класс для представления точки
class Point {
public:
    double x, y;

    Point(double _x, double _y) : x(_x), y(_y) {}
};

// Класс для представления круга
class Circle : public Figure {
    Point center;  // Координаты центра круга
    double radius; // Радиус круга

public:
    // Конструктор, принимающий цвет, координаты центра, радиус и дату создания
    Circle(string _color, Point _center, double _radius, string _date)
        : Figure(_color, _date), center(_center), radius(_radius) {}

    // Переопределенный метод вывода информации о круге
    void print() override {
        cout << "Circle: Color=" << color << ", Center=(" << center.x << ", " << center.y << "), Radius=" << radius << ", Year=" << getYear() << endl;
    }

    // Переопределенная функция для проверки условий (>, <, =)
    bool matchesCondition(const string& condition) const override {
        return Figure::matchesCondition(condition);
    }
};

// Аналогичные классы для прямоугольника и треугольника
class Rectangle : public Figure {
    Point p1, p2;

public:
    Rectangle(string _color, Point _p1, Point _p2, string _date)
        : Figure(_color, _date), p1(_p1), p2(_p2) {}

    void print() override {
        cout << "Rectangle: Color=" << color << ", Point1=(" << p1.x << ", " << p1.y << "), Point2=(" << p2.x << ", " << p2.y << "), Year=" << getYear() << endl;
    }

    bool matchesCondition(const string& condition) const override {
        return Figure::matchesCondition(condition);
    }
};

class Triangle : public Figure {
    Point p1, p2, p3;

public:
    Triangle(string _color, Point _p1, Point _p2, Point _p3, string _date)
        : Figure(_color, _date), p1(_p1), p2(_p2), p3(_p3) {}

    void print() override {
        cout << "Triangle: Color=" << color << ", Point1=(" << p1.x << ", " << p1.y << "), Point2=(" << p2.x << ", " << p2.y << "), Point3=(" << p3.x << ", " << p3.y << "), Year=" << getYear() << endl;
    }

    bool matchesCondition(const string& condition) const override {
        return Figure::matchesCondition(condition);
    }
};

struct Node {
    Node* next;
    Figure* data;
    
    Node(Figure* figure) {
        data = figure;
        next = nullptr;
    }
};

class List {
    Node* head;
    Node* tail;
public:
    List() : head(nullptr) {}
    
    void push_back(Node* figure) {
        if (!head) {
            head = figure;
            head->next = head;
            tail = head; // Устанавливаем tail в случае пустого списка
        } else {
            Node* last = get_prev(head);
            last->next = figure;
            figure->next = head;
            tail = figure; // Обновляем tail при добавлении новой фигуры
        }
    }
    
    void pop_front() {
        if (head == nullptr) {
            return;
        }
        
        if (head->next == head) {
            delete head;
            tail = nullptr;
            head = nullptr;
        }
        
        auto old_head = head;
        
        auto last = get_prev(head);
        
        head = head->next;
        last->next = head;
        delete old_head;
    }
    
    Node* get_prev(Node* node) {
        if (head == nullptr) {
            return nullptr;
        }
        
        Node* prev = head;
        Node* current = head->next;
        
        do {
            if (current == node) {
                return prev;
            }
            
            prev = current;
            current = current->next;
        } while (current != head);
        
        if (current == node) {
            return prev;
            
        };
        
        return nullptr;
    }
    
    Node* remove(Node* node) {
        if (node == head) {
            pop_front();
            return head;
        }
        
        auto prev = get_prev(node);
        
        prev->next = node->next;
        delete node;
        return prev->next;
    }
    
    void removeIfMatches(const string& condition) {
        Node *ptr = head;
        
        do {
            if (ptr->data->matchesCondition(condition)) {
                ptr = remove(ptr);
                
                if (ptr == nullptr)
                    break;
                continue;
            }
            
            ptr = ptr->next;
            
            if (ptr == head) {
                break;
            }
        } while (true);
        
        if (ptr && ptr->data->matchesCondition(condition)) {
            ptr = remove(ptr);
        }
    }
    
    // Метод вывода всех фигур
    void printAll() const {
        if (!head) {
            return;
        }
        Node* current = head;
        do {
            if (current == nullptr)
                break;
            current->data->print();
            current = current->next;
        } while (current != head);
        cout << endl;  // Добавляем перевод строки для корректного вывода.
    }
    
    void func_read(string& line)
    {
        
        stringstream sstr;
        sstr << line;
        
        string type, color, date;
        sstr >> type >> color;
        double x1, x2, x3, y1, y2, y3, radius;
        if (type == "circle") {
            sstr >> x1 >> y1 >> radius >> date;
            Point p1(x1,y1);
            push_back(new Node(new Circle(color, p1, radius, date)));
        } else if (type == "rectangle") {
            sstr >> x1 >> y1 >>x2>> y2  >> date;
            Point p1(x1,y1);
            Point p2(x2,y2);
            push_back(new Node(new Rectangle(color, p1, p2, date)));
        } else if (type == "triangle") {
            sstr >> x1 >> y1 >> x2 >> y2 >>x3 >> y3 >> date;
            Point p1(x1,y1);
            Point p2(x2,y2);
            Point p3(x3,y3);
            push_back(new Node(new Triangle(color, p1, p2, p3, date)));
        } else {
            cerr << "Wrong type figure" << endl;
        }
    }
};
int main() {
    List figureList;
    ifstream inputFile("/Users/alisa/Desktop/opppo/1_new/1_new/input.txt");

    if (!inputFile.is_open()) {
        cerr << "Unable to open input.txt" << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        string command;
        iss >> command;
        if (command == "ADD") {
            string line;
            getline(iss, line);
            figureList.func_read(line);
        }
        else if (command == "REM") {
            string condition;
            getline(iss, condition);
            figureList.removeIfMatches(condition);
        } else if (command == "PRINT") {
            figureList.printAll();
        } else {
            cerr << "Command Error: " << command << endl;
        }
    }

    return 0;
}
