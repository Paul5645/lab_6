#include <iostream>
#include <new>

template <typename T>
class List {
private:
    template <typename T>
    class Node {
    public:
        T data;
        Node* next;
        Node* prev;

        Node(T data = T(), Node* prev = nullptr, Node* next = nullptr) {
            this->data = data;
            this->prev = prev;
            this->next = next;
        }
    };

    Node<T> *top; // Указатель на первый элемент (вершина стека)
    int size; // Размер стека

public:

    int GetSize() const {
        return size;    
    };

    // Функция очистки стека
    void clear();

    // Функция добавления элемента в вершину стека
    bool Push(const T& element);

    // Функция удаления элемента из вершины стека
    bool Pop(T& element);

    bool Peek(T& element) const;

    // Конструктор по умолчанию
    List();

    // Конструктор копирования
    List(const List<T>& other);

    // Конструктор перемещения
    List(List<T>&& other);

    // Конструктор, принимающий на вход std::initializer_list
    List(std::initializer_list<T> ilist);

    // Деструктор
    ~List();

    // Переопределение оператора присваивания (копирования)
    List<T>& operator=(const List<T>& other);
    
    // Переопределение оператора присваивания (перемещение)
    List<T>& operator=(List<T>&& other);

    // Переопределение оператора присваивания (с использованием initializer_list)
    List<T>& operator=(std::initializer_list<T> ilist);


};

template<typename T>
List<T>::List()
{
    size = 0;
    top = nullptr;
}

template<typename T>
List<T>::~List() {
    clear();  // Очищаем список, используя функцию clear
}


template <typename T>
List<T>::List(const List<T>& other) : top(nullptr), size(0) {
    *this = other;  // Используем оператор присваивания
}

template <typename T>
List<T>::List(List<T>&& other) : top(nullptr), size(0) {
    *this = std::move(other);  // Используем оператор присваивания перемещения
}

template <typename T>
List<T>::List(std::initializer_list<T> ilist) : top(nullptr), size(0) {
    *this = ilist;  // Используем оператор присваивания
}


template <typename T>
List<T>& List<T>::operator=(const List<T>& other) {
    // Защита от самоприсваивания
    if (this != &other) {
        // Очищаем текущий список
        clear();

        // Проверяем, не пуст ли исходный список
        if (other.top != nullptr) {
            // Начинаем с элемента, который находится на "дне" исходного стека
            Node<T>* currentOther = other.top->next;

            do {
                // Копируем элементы из исходного списка в новый с помощью Push
                if (!Push(currentOther->data)) {
                    // Обработка ошибки выделения памяти, если Push возвращает false
                    clear();  // Очищаем уже скопированные элементы
                    throw std::bad_alloc();  // Выбрасываем исключение
                }
                currentOther = currentOther->next;
            } while (currentOther != other.top->next);  // Повторяем, пока не вернемся к начальному узлу
        }
    }

    return *this;
}

template <typename T>
List<T>& List<T>::operator=(List<T>&& other) {
    if (this != &other) {  // Защита от самоприсваивания
        clear();  // Очищаем текущий список

        // Перемещаем данные из other в текущий объект
        top = other.top;
        size = other.size;

        // Оставляем other в валидном, но пустом состоянии
        other.top = nullptr;
        other.size = 0;
    }

    return *this;
}


template <typename T>
List<T>& List<T>::operator=(std::initializer_list<T> ilist) {
    clear();  // Очистка текущего списка

    // Добавление каждого элемента из списка инициализации
    for (const T& item : ilist) {
        Push(item);
    }

    return *this;  // Возвращение ссылки на текущий объект
}

template<typename T>
void List<T>::clear() {
    Node<T>* prevNode;
    while (size > 0) {
        prevNode = top->prev; // Сохраняем ссылку на предыдущий элемент
        delete top; // Удаляем текущий узел
        top = prevNode; // Переходим к следующему узлу
        --size;
    }

    top = nullptr; // Обнуляем указатель на вершину стека
}

template<typename T>
bool List<T>::Push(const T& element) {
    // Создаем новый узел
    Node<T>* newNode = new(std::nothrow) Node<T>(element);

    // Проверяем, был ли узел успешно создан
    if (newNode == nullptr) {
        // Не удалось выделить память
        return false;
    }

    if (top == nullptr) {
        // Если список пуст, новый узел становится первым и единственным элементом
        newNode->next = newNode; // Ссылки на самого себя
        newNode->prev = newNode;
    }
    else {
        // Вставляем новый узел после текущего top
        newNode->next = top->next;
        newNode->prev = top;
        top->next->prev = newNode;
        top->next = newNode;
    }

    top = newNode; // Обновляем top для указания на новый узел
    size++;
    return true;
}

template<typename T>
bool List<T>::Pop(T& element) {
    // Проверяем, пуст ли стек
    if (top == nullptr) {
        // Стек пуст
        return false;
    }

    // Сохраняем данные верхнего элемента
    element = top->data;

    // Удаление верхнего элемента
    Node<T>* toDelete = top;

    // Если в стеке остался только один элемент
    if (top == top->next) {
        top = nullptr;
    }
    else {
        // В стеке более одного элемента
        top->prev->next = top->next;
        top->next->prev = top->prev;
        top = top->prev;  // Обновляем top
    }

    delete toDelete;  // Удаляем элемент
    --size;           // Уменьшаем размер стека
    return true;
}

template<typename T>
bool List<T>::Peek(T& element) const {
    // Проверяем, пуст ли стек
    if (top == nullptr) {
        // Стек пуст
        return false;
    }

    // Копируем данные верхнего элемента
    element = top->data;

    return true;
}

int main() {
    return 0;
}