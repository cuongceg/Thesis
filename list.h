#ifndef __list_header_
#define __list_header_

template <class T> class list{
	public:
		//////////////////////////////
		// Standard list operations //
		//////////////////////////////

		void insert(T element, int priority);
		bool del(T element);
		//concatenate();
		//split();
		T * findPrevious(T element);
		T * findNext(T element);
		//search();

		///////////////////////////////
		// Priority queue operations //
		///////////////////////////////
		
		bool updatePriority(T element, int priority);
		T findMin();

	private:
		struct S {
			T element;
			int priority;
		};
		std::vector<S> vec;
		size_t findIndex(T element);
};

#endif
