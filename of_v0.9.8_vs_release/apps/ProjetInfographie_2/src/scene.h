#pragma once

#include <vector>
#include <memory> //shared_ptr
#include <iostream>
#include "primitive.h"
#include "primitive2d.h"
#include "primitive3d.h"
#include "primitiveTopo.h"
#include "forme3d.h"
#include "extModel.h"


typedef std::shared_ptr<primitive> primitive_ptr;

class scene
{
public:
	class scene_iterator;

	scene();

	//temporaire
	void addElement(const primitive2d& p) {	addElement(0, p, true); }
	void addElement(const primitive3d& p) { addElement(0, p, true); }
	void addElement(const primitiveTopo& p) { addElement(0, p, true); }
	void addElement(const forme3d& p) { addElement(0, p, true); }
	void addElement(const extModel& p) { addElement(0, p, true); }

	void addElement(size_t index, const primitive2d& p, bool insertFirstChild);
	void addElement(size_t index, const primitive3d& p, bool insertFirstChild);
	void addElement(size_t index, const primitiveTopo& p, bool insertFirstChild);
	void addElement(size_t index, const forme3d& p, bool insertFirstChild);
	void addElement(size_t index, const extModel& p, bool insertFirstChild);
	void removeElement(size_t index);
	void clearElements();

	size_t getSize() const { return root->getSize(); }

	scene_iterator begin();
	scene_iterator end();

	int nbElements() { return root->getSize(); }
	
	friend std::ostream& operator<<(std::ostream& os, const scene& s);

private:
	void addElement(size_t index, primitive_ptr& p, bool insertFirstChild);

	class element {
	public:
		size_t getSize() const { return size; };
		size_t getIndex() const {	return index; };
		virtual void setIndex(size_t index) { this->index = index; };
		size_t getHeight() const { return height; };
		virtual void setHeight(size_t height) { this->height = height; };
		virtual std::string getType() const = 0;

		virtual size_t addElement(size_t index, primitive_ptr& p, bool insertFirstChild) = 0;
		virtual size_t removeElement(size_t index) = 0;
		virtual element* getElement(size_t index) = 0;

		friend std::ostream& operator<<(std::ostream& os, const element& e) { return e.print(os); };
	protected:
		size_t index;
		size_t size;
		size_t height;

		element(size_t index, size_t height);
		element(size_t index, size_t height, size_t size);

		virtual std::ostream& print(std::ostream& os) const;
	};
	
	typedef std::shared_ptr<element> element_ptr;

	class node : public element {
	public:
		node(size_t index, size_t height, primitive_ptr& p);

		std::string getType() const override;

		size_t addElement(size_t index, primitive_ptr& p, bool insertFirstChild) override;
		size_t removeElement(size_t index) override;
		element* getElement(size_t index) override;

		primitive_ptr content;
	protected:
		std::string contentType;
	};

	typedef std::shared_ptr<node> node_ptr;

	class group : public element {
	public:
		group(size_t index, size_t height);

		void setIndex(size_t index) override;
		void setHeight(size_t height) override;
		std::string getType() const override;

		size_t addElement(size_t index, primitive_ptr& p, bool insertFirstChild) override;
		size_t removeElement(size_t index) override;
		element* getElement(size_t index) override;

		std::vector<element_ptr> childrens;
	protected:
		std::ostream& print(std::ostream& os) const override;
	};

	typedef std::shared_ptr<group> group_ptr;

	group_ptr root;

public:
	class scene_iterator {
	public:
		scene_iterator(const group_ptr& root, size_t index);

		bool operator!=(const scene_iterator& it) { return p != it.p; }
		primitive& operator*() { return *p; }
		void operator++();

	private:
		group_ptr root;
		primitive_ptr p;
		size_t rootIndex;
	};
};

void test_scene();