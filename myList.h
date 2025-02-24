#pragma once

#include <memory>
#include <fstream>


template <typename T>
class Node {
public:
	T data;
	std::shared_ptr<Node<T>> next;
	std::shared_ptr<Node<T>> prev;

	Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};


template <typename T>
class MyList {
	std::shared_ptr<Node<T>> head;
	std::shared_ptr<Node<T>> tail;
	size_t list_size;

public:
	MyList() : head(nullptr), tail(nullptr), list_size(0) {}

	MyList(const MyList& other) {
		copyFrom(other);
	}

	~MyList() {
		clear();
	}

	size_t size() const {
		return list_size;
	}

	MyList& operator=(const MyList& other) {
		if (this != &other) {
			copyFrom(other);
		}
	}

	void push_front(const T& value) {
		auto newNode = std::make_shared<Node<T>>(value);
		if (!head) {
			head = newNode;
			tail = head;
		}
		else {
			newNode->next = head;
			head->prev = newNode;
			head = newNode;
		}
		list_size++;
	}

	void pop_front() {
		if (!head) {
			return;
		}
		else if (head == tail) {
			head = nullptr;
			tail = nullptr;
			list_size--;
		}
		else {
			head = head->next;
			head->prev = nullptr;
			list_size--;
		}
	}

	void push_back(const T& value) {
		auto newNode = std::make_shared<Node<T>>(value);
		if (!head) {
			head = newNode;
			tail = head;
		}
		else {
			newNode->prev = tail;
			tail->next = newNode;
			tail = newNode;
		}
		list_size++;
	}

	void pop_back() {
		if (!head) {
			return;
		}
		else if (head == tail) {
			head = nullptr;
			tail = nullptr;
			list_size--;
		}
		else {
			tail = tail->prev;
			tail->next = nullptr;
			list_size--;
		}
	}

	void clear() {
		while (head) {
			pop_back();
		}
	}

	T at(int index) const {
		auto current = head;
		
		for (size_t i = 0; current && i < index; i++) {
			current = current->next;
		}

		if (!current) {
			throw std::out_of_range("index out of range");
		}

		return current->data;
	}

	void removeAt(size_t position) {
		if (!head) {
			return;
		}

		auto current = head;
		size_t index = 0;

		while (current && index < position) {
			current = current->next;
			index++;
		}

		if (!current) {
			return;
		}

		if (current->prev) {
			current->prev->next = current->next;
		}
		else {
			head = current->next;
		}

		if (current->next) {
			current->next->prev = current->prev;
		}
		else {
			tail = current->prev;
		}

		list_size--;
	}

	void serialize(const std::string& filename) {
		std::ofstream file(filename);
		auto current = head;
		while (current) {
			file << current->data << " ";
			current = current->next;
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const MyList& list) {
		auto current = list.head;
		while (current) {
			os << current->data << " ";
			current = current->next;
		}
		return os;
	}

private:
	void copyFrom(const MyList& other) {
		head = nullptr;
		tail = nullptr;

		std::shared_ptr<Node<T>> current = other.head;
		while (current) {
			push_back(current->data);
			current = current->next;
		}
	}
};
