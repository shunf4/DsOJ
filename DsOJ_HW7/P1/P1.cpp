/*1652270 CS2 fengshun*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sstream>

template <typename VertexElem, typename EdgeWeight>
class Graph
{
public:
	bool directed;
	bool weighted;
	size_t vertexNum;
	EdgeWeight defaultWeight;

	Graph()
	{
		setDirected(false);
		setWeighted(false);
		setNonDirectWeight(1);
		vertexNum = 0;
	}

	Graph(bool directed, bool weighted, EdgeWeight defaultWeight)
	{
		setDirected(directed);
		setWeighted(weighted);
		setNonDirectWeight(defaultWeight);
		vertexNum = 0;
	}

	void setNonDirectWeight(EdgeWeight defaultWeight)
	{
		this->defaultWeight = defaultWeight;
	}

	void setDirected(bool d)
	{
		directed = d;
	}

	void setWeighted(bool w)
	{
		weighted = w;
	}
};

template <typename E>
class Matrix
{
private:
	E * matrix_array;
	
public:
	size_t rows;
	size_t cols;

	Matrix()
	{
		rows = 0;
		cols = 0;
		matrix_array = NULL;
	}
	void init(size_t rows, size_t cols, E* arr = NULL)
	{
		if (matrix_array)
			delete[] matrix_array;
		matrix_array = new E[rows * cols]();
		this->rows = rows;
		this->cols = cols;

		if(arr)
#ifdef _MSC_VER
			memcpy_s(matrix_array, sizeof(E) * rows * cols, arr, sizeof(E) * rows * cols);
#else
			memcpy(matrix_array, arr, sizeof(E) * rows * cols);
#endif
	}
	Matrix(unsigned rows, unsigned cols)
	{
		init(rows, cols);
	}
	Matrix(unsigned rows, unsigned cols, E * arr)
	{
		init(rows, cols, arr);
	}
	~Matrix()
	{
		if (matrix_array)
			delete[] matrix_array;
	}
	E * operator[](size_t i)
	{
		if (i >= rows)
			throw "INDEX OUT OF RANGE";
		return matrix_array + i * cols;
	}

	void print(std::ostream&out)
	{
		for (size_t r = 0; r < rows; r++)
		{
			for (size_t c = 0; c < cols; c++)
			{
				out << std::setw(3) << " " << (*this)[r][c];
			}
			out << std::endl;
		}
	}
};

template <typename E>
struct Node
{
	E data;
	Node<E> *next;
	Node()
	{
		next = NULL;
	}
	~Node()
	{
		if(next)
			delete next;
	}
};

template <typename EdgeWeight>
struct WeightedDest
{
	size_t vi;
	EdgeWeight we;
};

template <typename VertexElem, typename EdgeWeight>
struct AdjListElem
{
	VertexElem v;
	Node<WeightedDest<EdgeWeight>> *adjV;

	AdjListElem()
	{
		adjV = NULL;
	}

	~AdjListElem()
	{
		if (adjV)
			delete adjV;
	}

	void print(std::ostream &out, bool printWeight)
	{
		out << v << "-->";
		auto aP = adjV;
		while (aP)
		{
			if (aP != adjV)
				out << ' ';
			out << aP->data.vi;
			if (printWeight)
				out << ',' << aP->data.we;
			aP = aP->next;
		}
	}
};

template <typename VertexElem, typename EdgeWeight>
class Graph_Matrix : public Graph<VertexElem, EdgeWeight>
{
private:
	Matrix<EdgeWeight> e;  
	VertexElem * v;
	
public:
	Graph_Matrix(bool directed, bool weighted, EdgeWeight defaultWeight)
	{
		Graph<VertexElem, EdgeWeight>::Graph(directed, weighted, defaultWeight);
		v = NULL;
	}

	Graph_Matrix() : Graph<VertexElem, EdgeWeight>()
	{
		v = NULL;
	}

	size_t indexOfVertex(const VertexElem &vtx) const
	{
		for (size_t i = 0; i < e.cols; i++)
		{
			if (vtx == v[i])
				return i;
		}
		throw "NOT FOUND";
	}

	~Graph_Matrix()
	{
		if (v)
			delete [] v;
	}

	template <typename VertexElem_, typename EdgeWeight_>
	friend std::istream& operator>> (std::istream &in, Graph_Matrix<VertexElem_, EdgeWeight_> &gm);
	template <typename VertexElem_, typename EdgeWeight_>
	friend std::ostream& operator << (std::ostream &out, Graph_Matrix<VertexElem_, EdgeWeight_> &gm);

	void printVertex(std::ostream& out)
	{
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			if (i)
				out << ' ';
			out << v[i];
		}
	}
};

template <typename VertexElem, typename EdgeWeight>
class Graph_List : public Graph<VertexElem, EdgeWeight>
{
private:
	AdjListElem<VertexElem, EdgeWeight> * v;

public:
	Graph_List(bool directed, bool weighted, EdgeWeight defaultWeight) : Graph<VertexElem, EdgeWeight>(directed, weighted, defaultWeight)
	{
		v = NULL;
	}

	Graph_List() : Graph<VertexElem, EdgeWeight>()
	{
		v = NULL;
	}

	~Graph_List()
	{
		if (v)
			delete [] v;
	}

	size_t indexOfVertex(const VertexElem &vtx) const
	{
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			if (vtx == v[i].v)
				return i;
		}
		throw "NOT FOUND";
	}

	template <typename VertexElem_, typename EdgeWeight_>
	friend std::istream& operator >> (std::istream &in, Graph_List<VertexElem_, EdgeWeight_> &gm);
	template <typename VertexElem_, typename EdgeWeight_>
	friend std::ostream& operator << (std::ostream &out, Graph_List<VertexElem_, EdgeWeight_> &gm);

	void printVertex(std::ostream& out)
	{
		for (size_t i = 0; i < Graph<VertexElem, EdgeWeight>::vertexNum; i++)
		{
			if (i)
				out << ' ';
			out << v[i].v;
		}
	}
};



typedef enum { NONE, DIRECTED, DIRECTED_NET, UNDIRECTED, UNDIRECTED_NET } GraphType;

template <typename VertexElem, typename EdgeWeight>
std::istream & operator >> (std::istream & in, Graph_Matrix<VertexElem, EdgeWeight> &gm)
{
	if (gm.v)
		delete[]gm.v;
	
	GraphType gt;
	in >> reinterpret_cast<unsigned &>(gt);

	switch (gt)
	{
		case 1:
			gm.setDirected(true);
			gm.setWeighted(false);
			gm.setNonDirectWeight(1);
			break;
		case 2:
			gm.setDirected(true);
			gm.setWeighted(true);
			break;
		case 3:
			gm.setDirected(false);
			gm.setWeighted(false);
			gm.setNonDirectWeight(1);
			break;
		case 4:
			gm.setDirected(false);
			gm.setWeighted(true);
			break;
	}

	size_t vCnt, eCnt;
	in >> vCnt >> eCnt;

	gm.vertexNum = vCnt;
	
	gm.v = new VertexElem[vCnt];
	gm.e.init(vCnt, vCnt);
	
	for (size_t i = 0; i < vCnt; i++)
	{
		in >> gm.v[i];
	}

	VertexElem vS, vE;
	EdgeWeight eW;

	eW = gm.defaultWeight;

	for (size_t i = 0; i < eCnt; i++)
	{
		in >> vS >> vE;
		if (gm.weighted)
		{
			in >> eW;
		}
		
		gm.e[gm.indexOfVertex(vS)][gm.indexOfVertex(vE)] = eW;
		if (!gm.directed)
		{
			gm.e[gm.indexOfVertex(vE)][gm.indexOfVertex(vS)] = eW;
		}
	}
	return in;
}

template<typename VertexElem_, typename EdgeWeight_>
std::ostream & operator<<(std::ostream & out, Graph_Matrix<VertexElem_, EdgeWeight_>& gm)
{
	gm.e.print(out);
	return out;
}

template <typename VertexElem, typename EdgeWeight>
std::istream & operator >> (std::istream & in, Graph_List<VertexElem, EdgeWeight> &ga)
{
	if (ga.v)
		delete[]ga.v;

	GraphType gt;
	in >> reinterpret_cast<unsigned &>(gt);

	switch (gt)
	{
		case 1:
			ga.setDirected(true);
			ga.setWeighted(false);
			ga.setNonDirectWeight(1);
			break;
		case 2:
			ga.setDirected(true);
			ga.setWeighted(true);
			break;
		case 3:
			ga.setDirected(false);
			ga.setWeighted(false);
			ga.setNonDirectWeight(1);
			break;
		case 4:
			ga.setDirected(false);
			ga.setWeighted(true);
			break;
	}

	size_t vCnt, eCnt;
	in >> vCnt >> eCnt;

	ga.vertexNum = vCnt;

	ga.v = new AdjListElem<VertexElem, EdgeWeight>[vCnt]();


	for (size_t i = 0; i < vCnt; i++)
	{
		in >> ga.v[i].v;
	}

	VertexElem vS, vE;
	EdgeWeight eW;

	eW = ga.defaultWeight;
	Node<WeightedDest<EdgeWeight>> *curNP, *newNP;

	size_t vsi;
	size_t vei;

	for (size_t i = 0; i < eCnt; i++)
	{
		in >> vS >> vE;
		vsi = ga.indexOfVertex(vS);
		vei = ga.indexOfVertex(vE);
		if (ga.weighted)
		{
			in >> eW;
		}

		curNP = ga.v[vsi].adjV;
		newNP = new Node<WeightedDest<EdgeWeight>>;
		newNP->next = curNP;
		newNP->data.vi = vei;
		newNP->data.we = eW;

		ga.v[vsi].adjV = newNP;

		if (!ga.directed)
		{
			curNP = ga.v[vei].adjV;
			newNP = new Node<WeightedDest<EdgeWeight>>;
			newNP->next = curNP;
			newNP->data.vi = vsi;
			newNP->data.we = eW;
			ga.v[vei].adjV = newNP;
		}
	}
	return in;
}

template<typename VertexElem_, typename EdgeWeight_>
std::ostream & operator<<(std::ostream & out, Graph_List<VertexElem_, EdgeWeight_>& ga)
{
	for (size_t i = 0; i < ga.vertexNum; i++)
	{
		ga.v[i].print(out, ga.weighted);
		out << std::endl;
	}
	return out;
}

#define FS_INFILE
//#define FS_OUTFILE
int main()
{
#if(defined(_FS_DEBUG) && defined(FS_INFILE))
	std::ifstream f0;
	f0.open("p1.txt", std::ios::in);
	auto cinbackup = std::cin.rdbuf();
	std::cin.set_rdbuf(f0.rdbuf());
#endif

#if(defined(_FS_DEBUG) && defined(FS_OUTFILE))

	std::ofstream f1;
	f1.open("out.txt", std::ios::out);
	auto coutbackup = std::cout.rdbuf();
	std::cout.set_rdbuf(f1.rdbuf());
#endif

	std::stringstream mem;
	
	while (!std::cin.eof())
	{
		mem.put(std::cin.get());
	}

	mem.seekg(std::ios::beg);
	Graph_Matrix<char, int> g0;
	//Graph_Adj<char, int> g;
	mem >> g0;

	g0.printVertex(std::cout);
	std::cout << std::endl;

	std::cout << g0;

	mem.seekg(std::ios::beg);
	//Graph_Matrix<char, int> g;
	Graph_List<char, int> g1;
	mem >> g1;

	//g1.printVertex(std::cout);
	//std::cout << std::endl;

	std::cout << g1;


#if(defined(_FS_DEBUG) && defined(FS_INFILE))
	f0.close();
#endif

#if(defined(_FS_DEBUG) && defined(FS_OUTFILE))
	f1.close();
#endif
	return 0;
}

