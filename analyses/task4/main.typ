#import "../conf.typ": conf

#show: conf.with(
  meta: (
    title: "Анализ красно-чёрного дерева",
    author: "Кудяков Артём Александрович",
    group: "251",
    city: "Саратов",
    year: 2025,
  )
)

= Код программы

```cpp
#pragma once

#include <concepts>
#include <memory>
#include <optional>

template <std::totally_ordered T>
class RedBlackTree {
public:
    RedBlackTree() {
        NIL = std::make_shared<Node>();
        root = NIL;
    };

    void insert(const T& value) {
        // Если дерево пустое, создаём корень
        if (root->isNil()) {
            // В КЧД корень обязательно чёрный
            root = std::make_shared<Node>(value, Node::Color::BLACK);

            // Задаём фиктивных детей
            root->left = NIL;
            root->right = NIL;

            // Задаём пустой указатель в качестве родителя
            root->parent = std::weak_ptr<Node>();

            return;
        }

        // Если же дерево не пустое, то мы создаём новый красный узел
        auto node = std::make_shared<Node>(value, Node::Color::RED);
        node->left = NIL;
        node->right = NIL;

        std::shared_ptr<Node> p = root;
        std::shared_ptr<Node> q = NIL;

        // Спускаемся вниз, пока не дойдём до подходящего листа
        while (!p->isNil()) {
            q = p;
            if (*(p->value) < value) {
                p = p->right;
            } else {
                p = p->left;
            }
        }
        node->parent = q;

        // Добавляем новый узел красного цвета
        if (*(q->value) < value) {
            q->right = node;
        } else {
            q->left = node;
        }

        fixAfterInsert(node);
    }

    bool remove(const T& value) {
        // Поиск узла для удаления встроен в метод
        auto nodeToDelete = root;
        while (!nodeToDelete->isNil()) {
            if (value == nodeToDelete->getValue()) {
                break;
            } else if (value < nodeToDelete->getValue()) {
                nodeToDelete = nodeToDelete->left;
            } else {
                nodeToDelete = nodeToDelete->right;
            }
        }

        // Если узел не найден
        if (nodeToDelete->isNil()) {
            return false;
        }

        // y - узел, который будет физически удалён
        // x - узел, который займёт место y
        std::shared_ptr<Node> y, x;

        // Определяем узел для физического удаления
        if (nodeToDelete->left->isNil() || nodeToDelete->right->isNil()) {
            // У узла 0 или 1 ребёнок - удаляем сам узел
            y = nodeToDelete;
        } else {
            // У узла 2 ребёнка - ищем преемника (встроенный поиск)
            y = nodeToDelete->right;
            while (!y->left->isNil()) {
                y = y->left;
            }
        }

        // Определяем узел, который займёт место y
        if (!y->left->isNil()) {
            x = y->left;
        } else {
            x = y->right;
        }

        // Сохраняем родителя y для случая, когда x будет NIL
        auto yParent = y->parent.lock();

        // Связываем x с родителем y
        if (!x->isNil()) {
            x->parent = y->parent;
        }

        if (!yParent) {
            // y был корнем
            root = x;
        } else {
            // Подключаем x к родителю y
            if (y == yParent->left) {
                yParent->left = x;
            } else {
                yParent->right = x;
            }
        }

        // Сохраняем цвет удаляемого узла
        auto yOriginalColor = y->color;

        // Если y не тот узел, который мы хотели удалить,
        // копируем значение y в nodeToDelete
        if (y != nodeToDelete) {
            nodeToDelete->value = y->value;
        }

        // Если удалённый узел был чёрным, нужна корректировка
        if (yOriginalColor == Node::BLACK) {
            fixAfterRemove(x, yParent);
        }

        return true;
    }
private:
    struct Node {
        enum Color { RED, BLACK };

        std::optional<T> value;
        Color color;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent;

        Node(const T& value, Color color = RED) : value(value), color(color) {}
        Node() : color(BLACK) {}

        bool isNil() const { return !value.has_value(); }
        bool isRoot() const { return parent.expired(); }
        const T& getValue() const { return value.value(); }
    };

    std::shared_ptr<Node> NIL;
    std::shared_ptr<Node> root;

    // Метод, который восстанавливает свойства красно-чёрного дерева после вставки узла
    void fixAfterInsert(std::shared_ptr<Node> node) {
        // Если узел является корнем
        if (node->isRoot()) {
            // то мы его должны перекрасить в чёрный цвет
            node->color = Node::BLACK;
            return;
        }

        while (true) {
            auto parent = node->parent.lock();
            if (!parent || parent->color == Node::BLACK) {
                // Если наш отец чёрный, то мы не нарушаем никаких правил
                // => мы можем закончить наш цикл, КЧД исправлено
                break;
            }

            auto grandfather = parent->parent.lock();
            if (!grandfather) {
                break;
            }

            if (grandfather->left == parent) {
                // Если родитель -- левый ребёнок дедушки,
                // то дядя -- это правый рёбёнок дедушки
                auto uncle = grandfather->right;

                // Если у нас есть красный дядя, то начинаем перекрашивание
                if (!uncle->isNil() && uncle->color == Node::Color::RED) {
                    parent->color = Node::Color::BLACK;
                    uncle->color = Node::Color::BLACK;
                    grandfather->color = Node::Color::RED;

                    // ОСТОРОЖНО
                    node = grandfather;
                } else {
                    // Если же наш дядя чёрный, то начинаем повороты

                    // Если node -- правый сын
                    if (node == parent->right) {
                        // то мы поворачиваем влево
                        node = parent;
                        leftRotate(node);
                    }

                    parent = node->parent.lock();
                    grandfather = parent->parent.lock();

                    parent->color = Node::Color::BLACK;
                    grandfather->color = Node::Color::RED;
                    rightRotate(grandfather);
                }
            } else {
                // Если родитель -- правый ребёнок дедушки,
                // то дядя -- это левый рёбёнок дедушки

                auto uncle = grandfather->left;

                // Если у нас есть красный дядя, то начинаем перекрашивание
                if (!uncle->isNil() && uncle->color == Node::Color::RED) {
                    parent->color = Node::Color::BLACK;
                    uncle->color = Node::Color::BLACK;
                    grandfather->color = Node::Color::RED;

                    // ОСТОРОЖНО
                    node = grandfather;
                } else {
                    // Если же наш дядя чёрный, то начинаем повороты

                    // Если node -- левый сын
                    if (node == parent->left) {
                        // то мы поворачиваем влево

                        node = parent;
                        rightRotate(node);
                    }

                    parent = node->parent.lock();
                    grandfather = parent->parent.lock();
                    parent->color = Node::Color::BLACK;
                    grandfather->color = Node::Color::RED;

                    leftRotate(grandfather);
                }
            }
        }

        // Красим корень в чёрный
        root->color = Node::Color::BLACK;
    }

    void fixAfterRemove(std::shared_ptr<Node> x, std::shared_ptr<Node> xParent = nullptr) {
        while (x != root && (x->isNil() || x->color == Node::BLACK)) {
            auto parent = x->isNil() ? xParent : x->parent.lock();

            if (!parent) break;

            if (x == parent->left) {
                // x - левый ребёнок
                auto sibling = parent->right;

                // Случай 1: брат красный
                if (!sibling->isNil() && sibling->color == Node::RED) {
                    sibling->color = Node::BLACK;
                    parent->color = Node::RED;
                    leftRotate(parent);
                    sibling = parent->right;
                }

                // Случай 2: брат чёрный, оба его ребёнка чёрные
                if ((sibling->left->isNil() || sibling->left->color == Node::BLACK) &&
                    (sibling->right->isNil() || sibling->right->color == Node::BLACK)) {
                    if (!sibling->isNil()) {
                        sibling->color = Node::RED;
                    }
                    x = parent;
                    xParent = parent->parent.lock();
                } else {
                    // Случай 3: правый ребёнок брата чёрный, левый красный
                    if (sibling->right->isNil() || sibling->right->color == Node::BLACK) {
                        if (!sibling->left->isNil()) {
                            sibling->left->color = Node::BLACK;
                        }
                        sibling->color = Node::RED;
                        rightRotate(sibling);
                        sibling = parent->right;
                    }

                    // Случай 4: правый ребёнок брата красный
                    sibling->color = parent->color;
                    parent->color = Node::BLACK;
                    if (!sibling->right->isNil()) {
                        sibling->right->color = Node::BLACK;
                    }
                    leftRotate(parent);
                    x = root;
                    break;
                }
            } else {
                // x - правый ребёнок (симметричные случаи)
                auto sibling = parent->left;

                // Случай 1: брат красный
                if (!sibling->isNil() && sibling->color == Node::RED) {
                    sibling->color = Node::BLACK;
                    parent->color = Node::RED;
                    rightRotate(parent);
                    sibling = parent->left;
                }

                // Случай 2: брат чёрный, оба его ребёнка чёрные
                if ((sibling->left->isNil() || sibling->left->color == Node::BLACK) &&
                    (sibling->right->isNil() || sibling->right->color == Node::BLACK)) {
                    if (!sibling->isNil()) {
                        sibling->color = Node::RED;
                    }
                    x = parent;
                    xParent = parent->parent.lock();
                } else {
                    // Случай 3: левый ребёнок брата чёрный, правый красный
                    if (sibling->left->isNil() || sibling->left->color == Node::BLACK) {
                        if (!sibling->right->isNil()) {
                            sibling->right->color = Node::BLACK;
                        }
                        sibling->color = Node::RED;
                        leftRotate(sibling);
                        sibling = parent->left;
                    }

                    // Случай 4: левый ребёнок брата красный
                    sibling->color = parent->color;
                    parent->color = Node::BLACK;
                    if (!sibling->left->isNil()) {
                        sibling->left->color = Node::BLACK;
                    }
                    rightRotate(parent);
                    x = root;
                    break;
                }
            }
        }

        if (!x->isNil()) {
            x->color = Node::BLACK;
        }

    }

    void leftRotate(std::shared_ptr<Node> x) {
        // Правый ребёнок становится новым корнем поддерева
        auto y = x->right;

        // Левый ребёнок y становится правым ребёнком x
        x->right = y->left;
        if (y->left && !y->left->isNil()) {
            y->left->parent = x; // Обновляем родителя левого ребёнка y
        }

        // y занимает место x
        y->parent = x->parent;

        // Обновляем родителя x
        if (auto parent = x->parent.lock()) {
            if (x == parent->left) {
                parent->left = y;
            } else {
                parent->right = y;
            }
        } else {
            // Если мы не смогли найти родителя x, значит x был корнем
            root = y;
        }

        // Делаем x левым ребёнком y
        y->left = x;
        x->parent = y;
    }

    void rightRotate(std::shared_ptr<Node> x) {
        // Левый ребёнок становится новым корнем поддерева
        auto y = x->left;

        // Правый ребёнок y становится левым ребёнком x
        x->left = y->right;
        if (y->right && !y->right->isNil()) {
            y->right->parent = x; // Обновляем родителя правого ребёнка y
        }

        // y занимает место x
        y->parent = x->parent;

        // Обновляем родителя x
        if (auto parent = x->parent.lock()) {
            if (x == parent->left) {
                parent->left = y;
            } else {
                parent->right = y;
            }
        } else {
            // Если мы не смогли найти родителя x, значит x был корнем
            root = y;
        }

        // Делаем y правым ребёнком x
        y->right = x;
        x->parent = y;
    }
};
```

#pagebreak()

= Анализ сложности

== Левый и правый поворот

Выполняеются за $O(1)$.

== Поиск

Происходит, как в обычном бинарном дереве поиска, но поскольку КЧД всегда
сбалансированное, поиск выполняется за $O(log n)$.

== Обходы

Выполняются за $O(n)$, как в обычном дереве бинарного поиска.

== Вставка

+ Если *дерево пустое*, мы просто устанавливаем чёрный корень за $O(1)$
+ Если *дерево не пустое*, вставка происходит в несколько этапов:
  + Производим *вставку* красного узла, как в обычном бинарном дереве поиска.
    Так как КЧД всегда сбалансированное, вставка выполняется за $O(log n)$.
  + Производим *восстановление свойств КЧД*:
    + В худшем случае нам потребуется произвести два поворота: $O(1) + O(1) = O(1)$.
    + Если нам необходимо перекрасить другие узлы, то она будет идти до корня.
      Высота дерева $log n$, а значит перекраска займёт $O(log n)$.

*Итоговая сложность:* $O(log n)$

== Удаление

+ *Нахождение узла*, как и ранее, происходит за $O(log n)$.
+ *Восстановление свойств*:
  + В худшем случае нам потребуется произвести три поворота: $O(1) + O(1) + O(1) = O(1)$.
  + Перекраска узлов происходит аналогично случаю вставки, то есть за $O(log n)$.

*Итоговая сложность:* $O(log n)$
