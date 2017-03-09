#pragma once

#include <vector>
#include <memory> //shared_ptr
#include "primitive.h"


class Scene
{
public:
	Scene();
private:
	class SceneElement {
	public:
		int getSize();
		int getIndex();
		void setIndex(int index);

		void setVisible(bool visible);
		bool isVisible();
		void setLocked(bool locked);
		bool isLocked();

		virtual void addElement(std::shared_ptr<primitive>& e) { };
		virtual void addElement(int index, std::shared_ptr<primitive>& e);
		virtual void removeElement() { };
		virtual void removeElement(int index);
	protected:
		int size;
		int index;

		bool visible;
		bool locked;

		SceneElement(int index);
		SceneElement(int index, int size);
	};

	class SceneNode : SceneElement {
	public:
		SceneNode(int index, std::shared_ptr<primitive>& content);

		void addElement(int index, std::shared_ptr<primitive>& e);
		void removeElement(int index);
	protected:
		std::shared_ptr<primitive> content;
	private:
		void addElement(std::shared_ptr<primitive>& e);
		void removeElement();
	};

	class SceneGroup : SceneElement {
	public:
		SceneGroup(int index);

		void setVisible(bool visible);
		void setLocked(bool locked);
		void setIndex(int index);

		void addElement(int index, std::shared_ptr<primitive>& e);
		void removeElement(int index);
	protected:
		std::vector<std::shared_ptr<SceneElement>> childrens;
	private:
		void addElement(std::shared_ptr<primitive>& e);
		void removeElement();
	};

	SceneGroup root;
	int currentPosition;
	std::shared_ptr<SceneElement> selectedElement;

	void addElement(std::shared_ptr<primitive>& e);
	void addElement(int index, std::shared_ptr<primitive>& e);

	void selectElement(int index);

	void rebuildIndex(int from);
};

