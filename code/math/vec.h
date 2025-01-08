

template <int DIM, class T>
class vectorN
{
public:
    vectorN()
    {
        for (unsigned i = 0; i < DIM; i++)
        {
            data[i] = T(0);
        }
    }
    template <class T2>
    explicit vectorN(const vectorN<DIM, T2> rhs)
    {
        for (unsigned i = 0; i < DIM; i++)
        {
            data[i] = rhs[i];
        }
    }

protected:
    T[] data;
};