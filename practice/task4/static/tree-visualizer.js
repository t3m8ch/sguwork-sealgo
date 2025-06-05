class RedBlackTreeVisualizer {
  constructor() {
    this.baseUrl = "http://localhost:8080/api";
    this.currentTree = null;
    this.init();
  }

  async init() {
    await this.loadTree();
    this.setupEventListeners();
  }

  setupEventListeners() {
    const input = document.getElementById("value-input");
    input.addEventListener("keypress", (e) => {
      if (e.key === "Enter") {
        this.insertValue();
      }
    });

    const randomCountInput = document.getElementById("random-count");
    randomCountInput.addEventListener("keypress", (e) => {
      if (e.key === "Enter") {
        this.addRandomValues();
      }
    });
  }

  async loadTree() {
    try {
      const response = await fetch(`${this.baseUrl}/tree`);
      const data = await response.json();

      if (data.success) {
        this.currentTree = data.tree_state;
        this.renderTree();
        this.updateStats();
      }
    } catch (error) {
      this.showStatus("Ошибка загрузки дерева: " + error.message, "error");
    }
  }

  async insertValue() {
    const input = document.getElementById("value-input");
    const value = parseInt(input.value);

    if (isNaN(value)) {
      this.showStatus("Пожалуйста, введите корректное число", "error");
      return;
    }

    this.setButtonsEnabled(false);

    try {
      const response = await fetch(`${this.baseUrl}/tree/insert`, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ value: value }),
      });

      const data = await response.json();

      if (data.success) {
        this.currentTree = data.tree_state;
        this.showStatus(`Значение ${value} успешно добавлено`, "success");
        this.renderTree();
        this.updateStats();
        input.value = "";
      } else {
        this.showStatus("Ошибка: " + data.message, "error");
      }
    } catch (error) {
      this.showStatus("Ошибка сети: " + error.message, "error");
    } finally {
      this.setButtonsEnabled(true);
    }
  }

  async removeValue() {
    const input = document.getElementById("value-input");
    const value = parseInt(input.value);

    if (isNaN(value)) {
      this.showStatus("Пожалуйста, введите корректное число", "error");
      return;
    }

    this.setButtonsEnabled(false);

    try {
      const response = await fetch(`${this.baseUrl}/tree/remove`, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ value: value }),
      });

      const data = await response.json();

      if (data.success) {
        this.currentTree = data.tree_state;
        this.showStatus(`Значение ${value} успешно удалено`, "success");
        this.renderTree();
        this.updateStats();
        input.value = "";
      } else {
        this.showStatus("Ошибка: " + data.message, "error");
      }
    } catch (error) {
      this.showStatus("Ошибка сети: " + error.message, "error");
    } finally {
      this.setButtonsEnabled(true);
    }
  }

  async clearTree() {
    if (!confirm("Вы уверены, что хотите очистить всё дерево?")) {
      return;
    }

    this.setButtonsEnabled(false);

    try {
      const response = await fetch(`${this.baseUrl}/tree/clear`, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
      });

      const data = await response.json();

      if (data.success) {
        this.currentTree = data.tree_state;
        this.showStatus("Дерево успешно очищено", "success");
        this.renderTree();
        this.updateStats();
      } else {
        this.showStatus("Ошибка: " + data.message, "error");
      }
    } catch (error) {
      this.showStatus("Ошибка сети: " + error.message, "error");
    } finally {
      this.setButtonsEnabled(true);
    }
  }

  // Новая функция для добавления случайных значений
  async addRandomValues() {
    const countInput = document.getElementById("random-count");
    const count = parseInt(countInput.value);

    if (isNaN(count) || count < 1 || count > 50) {
      this.showStatus("Пожалуйста, введите количество от 1 до 50", "error");
      return;
    }

    this.setButtonsEnabled(false);
    this.showStatus(`Добавляем ${count} случайных значений...`, "success");

    // Генерируем уникальные случайные числа
    const randomValues = this.generateUniqueRandomNumbers(count, 1, 100);
    let successCount = 0;

    for (let i = 0; i < randomValues.length; i++) {
      const value = randomValues[i];

      try {
        const response = await fetch(`${this.baseUrl}/tree/insert`, {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify({ value: value }),
        });

        const data = await response.json();

        if (data.success) {
          this.currentTree = data.tree_state;
          this.renderTree();
          this.updateStats();
          successCount++;

          // Обновляем статус с прогрессом
          this.showStatus(
            `Добавлено ${successCount} из ${count} значений...`,
            "success",
          );

          // Небольшая задержка для визуального эффекта
          await new Promise((resolve) => setTimeout(resolve, 150));
        }
      } catch (error) {
        console.error("Ошибка при вставке значения:", error);
      }
    }

    this.showStatus(
      `Успешно добавлено ${successCount} случайных значений!`,
      "success",
    );
    this.setButtonsEnabled(true);
  }

  // Генерация уникальных случайных чисел
  generateUniqueRandomNumbers(count, min, max) {
    const numbers = new Set();
    const range = max - min + 1;

    // Если запрашиваем больше чисел, чем возможно в диапазоне
    if (count > range) {
      this.showStatus(
        `Невозможно сгенерировать ${count} уникальных чисел в диапазоне ${min}-${max}`,
        "error",
      );
      return [];
    }

    while (numbers.size < count) {
      const randomNum = Math.floor(Math.random() * range) + min;
      numbers.add(randomNum);
    }

    return Array.from(numbers);
  }

  renderTree() {
    const container = document.getElementById("tree-container");

    if (!this.currentTree) {
      container.innerHTML =
        '<div class="loading">🌱 Дерево пустое. Добавьте значения для начала работы!</div>';
      return;
    }

    const svg = this.generateTreeVisualization(this.currentTree);
    container.innerHTML = svg;
  }

  generateTreeVisualization(nodeData) {
    if (!nodeData) {
      return '<div class="loading">🌱 Дерево пустое. Добавьте значения для начала работы!</div>';
    }

    const svgElement = document.createElementNS(
      "http://www.w3.org/2000/svg",
      "svg",
    );
    svgElement.setAttribute("width", "100%");
    svgElement.setAttribute("height", "600");
    svgElement.setAttribute("viewBox", "0 0 1200 600");

    this.renderNode(svgElement, nodeData, 600, 60, 300);

    return svgElement.outerHTML;
  }

  renderNode(svg, nodeData, x, y, xOffset) {
    if (!nodeData) return;

    // Рисуем связи с детьми сначала (чтобы они были под узлами)
    if (nodeData.left && nodeData.left.value !== undefined) {
      const leftLine = document.createElementNS(
        "http://www.w3.org/2000/svg",
        "line",
      );
      leftLine.setAttribute("x1", x);
      leftLine.setAttribute("y1", y + 30);
      leftLine.setAttribute("x2", x - xOffset);
      leftLine.setAttribute("y2", y + 90);
      leftLine.setAttribute("class", "tree-edge");
      svg.appendChild(leftLine);

      this.renderNode(svg, nodeData.left, x - xOffset, y + 100, xOffset / 1.8);
    }

    if (nodeData.right && nodeData.right.value !== undefined) {
      const rightLine = document.createElementNS(
        "http://www.w3.org/2000/svg",
        "line",
      );
      rightLine.setAttribute("x1", x);
      rightLine.setAttribute("y1", y + 30);
      rightLine.setAttribute("x2", x + xOffset);
      rightLine.setAttribute("y2", y + 90);
      rightLine.setAttribute("class", "tree-edge");
      svg.appendChild(rightLine);

      this.renderNode(svg, nodeData.right, x + xOffset, y + 100, xOffset / 1.8);
    }

    // Создаем узел
    const circle = document.createElementNS(
      "http://www.w3.org/2000/svg",
      "circle",
    );
    circle.setAttribute("cx", x);
    circle.setAttribute("cy", y);
    circle.setAttribute("r", 30);

    // Устанавливаем цвет узла
    if (nodeData.color === "red") {
      circle.setAttribute("class", "red-node");
    } else {
      circle.setAttribute("class", "black-node");
    }

    svg.appendChild(circle);

    // Добавляем текст значения
    const text = document.createElementNS("http://www.w3.org/2000/svg", "text");
    text.setAttribute("x", x);
    text.setAttribute("y", y);
    text.setAttribute("class", "node-text");
    text.textContent = nodeData.value;
    svg.appendChild(text);
  }

  updateStats() {
    const nodeCount = this.countNodes(this.currentTree);
    const height = this.calculateHeight(this.currentTree);
    const blackHeight = this.calculateBlackHeight(this.currentTree);

    document.getElementById("node-count").textContent = nodeCount;
    document.getElementById("tree-height").textContent = height;
    document.getElementById("black-height").textContent = blackHeight;
  }

  countNodes(node) {
    if (!node) return 0;
    return 1 + this.countNodes(node.left) + this.countNodes(node.right);
  }

  calculateHeight(node) {
    if (!node) return 0;
    return (
      1 +
      Math.max(
        this.calculateHeight(node.left),
        this.calculateHeight(node.right),
      )
    );
  }

  calculateBlackHeight(node) {
    if (!node) return 1;
    const leftHeight = this.calculateBlackHeight(node.left);
    const rightHeight = this.calculateBlackHeight(node.right);
    const blackIncrement = node.color === "black" ? 1 : 0;
    return Math.max(leftHeight, rightHeight) + blackIncrement;
  }

  showStatus(message, type) {
    const status = document.getElementById("status");
    status.textContent = message;
    status.className = `status ${type}`;
    status.style.display = "block";

    setTimeout(() => {
      status.style.display = "none";
    }, 3000);
  }

  setButtonsEnabled(enabled) {
    const buttons = ["insert-btn", "remove-btn", "clear-btn", "random-btn"];
    buttons.forEach((id) => {
      document.getElementById(id).disabled = !enabled;
    });
  }
}

// Глобальные функции для кнопок
let visualizer;

window.addEventListener("DOMContentLoaded", () => {
  visualizer = new RedBlackTreeVisualizer();
});

function insertValue() {
  visualizer.insertValue();
}

function removeValue() {
  visualizer.removeValue();
}

function clearTree() {
  visualizer.clearTree();
}

function addRandomValues() {
  visualizer.addRandomValues();
}
