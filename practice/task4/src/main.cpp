#include <crow.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <filesystem>
#include "tree.hpp"

// Простой класс для работы с вашей реализацией
class RedBlackTreeServer {
private:
    RedBlackTree<int> tree;
    int operationCounter = 0;

public:
    RedBlackTreeServer() = default;

    crow::response insert(int value) {
        try {
            tree.enableTracing("live");
            tree.insert(value);

            std::string filename = "insert_" + std::to_string(value) + "_" +
                                 std::to_string(++operationCounter) + ".json";
            tree.saveTrace(filename);

            nlohmann::json response;
            response["success"] = true;
            response["message"] = "Element inserted successfully";
            response["tree_state"] = tree.toJson();
            response["value"] = value;

            return crow::response(200, response.dump());
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["success"] = false;
            error["message"] = e.what();
            return crow::response(500, error.dump());
        }
    }

    crow::response remove(int value) {
        try {
            tree.enableTracing("live");
            bool removed = tree.remove(value);

            if (removed) {
                std::string filename = "remove_" + std::to_string(value) + "_" +
                                     std::to_string(++operationCounter) + ".json";
                tree.saveTrace(filename);
            }

            nlohmann::json response;
            response["success"] = removed;
            response["message"] = removed ? "Element removed successfully" : "Element not found";
            response["tree_state"] = tree.toJson();
            response["value"] = value;

            return crow::response(200, response.dump());
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["success"] = false;
            error["message"] = e.what();
            return crow::response(500, error.dump());
        }
    }

    crow::response getState() {
        try {
            nlohmann::json response;
            response["success"] = true;
            response["tree_state"] = tree.toJson();

            return crow::response(200, response.dump());
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["success"] = false;
            error["message"] = e.what();
            return crow::response(500, error.dump());
        }
    }

    crow::response clear() {
        try {
            tree = RedBlackTree<int>();
            operationCounter = 0;

            nlohmann::json response;
            response["success"] = true;
            response["message"] = "Tree cleared successfully";
            response["tree_state"] = tree.toJson();

            return crow::response(200, response.dump());
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["success"] = false;
            error["message"] = e.what();
            return crow::response(500, error.dump());
        }
    }
};

// Функция для загрузки файлов
std::string loadFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    // Используем простое приложение без middleware для избежания проблем с CORS
    crow::SimpleApp app;
    RedBlackTreeServer treeServer;

    // Создаем директории
    std::filesystem::create_directories("tracing/live");
    std::filesystem::create_directories("static");

    // Главная страница - интерактивный интерфейс
    CROW_ROUTE(app, "/").methods("GET"_method)([](const crow::request& req){
        std::string content = loadFile("static/index.html");
        if (content.empty()) {
            return crow::response(404, "index.html not found");
        }

        crow::response res(200, content);
        res.add_header("Content-Type", "text/html");
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });

    // Трассировщик
    CROW_ROUTE(app, "/tracer").methods("GET"_method)([](const crow::request& req){
        std::string content = loadFile("static/tracer.html");
        if (content.empty()) {
            return crow::response(404, "tracer.html not found");
        }

        crow::response res(200, content);
        res.add_header("Content-Type", "text/html");
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });

    // Статические файлы
    CROW_ROUTE(app, "/static/<string>").methods("GET"_method)([](const crow::request& req, const std::string& filename){
        std::string path = "static/" + filename;
        std::string content = loadFile(path);

        if (content.empty()) {
            return crow::response(404, "File not found");
        }

        crow::response res(200, content);

        // Определяем Content-Type
        if (filename.ends_with(".css")) {
            res.add_header("Content-Type", "text/css");
        } else if (filename.ends_with(".js")) {
            res.add_header("Content-Type", "application/javascript");
        } else if (filename.ends_with(".html")) {
            res.add_header("Content-Type", "text/html");
        }

        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });

    // API endpoints с CORS headers

    // OPTIONS для CORS preflight
    CROW_ROUTE(app, "/api/<path>").methods("OPTIONS"_method)([](const crow::request& req, const std::string& path){
        crow::response res(200);
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        res.add_header("Access-Control-Max-Age", "86400");
        res.add_header("Content-Length", "0");
        return res;
    });

    // Получить состояние дерева
    CROW_ROUTE(app, "/api/tree").methods("GET"_method)([&treeServer](const crow::request& req){
        auto response = treeServer.getState();
        response.add_header("Access-Control-Allow-Origin", "*");
        response.add_header("Content-Type", "application/json");
        return response;
    });

    // Вставить элемент
    CROW_ROUTE(app, "/api/tree/insert").methods("POST"_method)([&treeServer](const crow::request& req){
        try {
            auto json = nlohmann::json::parse(req.body);
            int value = json["value"];
            auto response = treeServer.insert(value);
            response.add_header("Access-Control-Allow-Origin", "*");
            response.add_header("Content-Type", "application/json");
            return response;
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["success"] = false;
            error["message"] = "Invalid JSON or missing 'value' field: " + std::string(e.what());

            crow::response res(400, error.dump());
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Content-Type", "application/json");
            return res;
        }
    });

    // Удалить элемент
    CROW_ROUTE(app, "/api/tree/remove").methods("POST"_method)([&treeServer](const crow::request& req){
        try {
            auto json = nlohmann::json::parse(req.body);
            int value = json["value"];
            auto response = treeServer.remove(value);
            response.add_header("Access-Control-Allow-Origin", "*");
            response.add_header("Content-Type", "application/json");
            return response;
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["success"] = false;
            error["message"] = "Invalid JSON or missing 'value' field: " + std::string(e.what());

            crow::response res(400, error.dump());
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Content-Type", "application/json");
            return res;
        }
    });

    // Очистить дерево
    CROW_ROUTE(app, "/api/tree/clear").methods("POST"_method)([&treeServer](const crow::request& req){
        auto response = treeServer.clear();
        response.add_header("Access-Control-Allow-Origin", "*");
        response.add_header("Content-Type", "application/json");
        return response;
    });

    // Список файлов трассировки
    CROW_ROUTE(app, "/api/traces").methods("GET"_method)([](const crow::request& req){
        nlohmann::json response;
        response["success"] = true;
        response["traces"] = nlohmann::json::array();

        try {
            std::string tracePath = "tracing/live";
            if (std::filesystem::exists(tracePath)) {
                for (const auto& entry : std::filesystem::directory_iterator(tracePath)) {
                    if (entry.path().extension() == ".json") {
                        response["traces"].push_back(entry.path().filename().string());
                    }
                }
            }
        } catch (const std::exception& e) {
            response["success"] = false;
            response["message"] = e.what();
        }

        crow::response res(200, response.dump());
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Content-Type", "application/json");
        return res;
    });

    // Загрузить конкретный файл трассировки
    CROW_ROUTE(app, "/api/traces/<string>").methods("GET"_method)([](const crow::request& req, const std::string& filename){
        std::string filePath = "tracing/live/" + filename;
        std::string content = loadFile(filePath);

        if (content.empty()) {
            nlohmann::json error;
            error["success"] = false;
            error["message"] = "Trace file not found";

            crow::response res(404, error.dump());
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Content-Type", "application/json");
            return res;
        }

        crow::response res(200, content);
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Content-Type", "application/json");
        return res;
    });

    std::cout << "🌳 Starting Red-Black Tree Visualizer" << std::endl;
    std::cout << "📊 Interactive interface: http://localhost:8080" << std::endl;
    std::cout << "🔍 Tracer interface: http://localhost:8080/tracer" << std::endl;
    std::cout << "📁 Trace files saved to: tracing/live/" << std::endl;

    app.port(8080).multithreaded().run();

    return 0;
}
