#include "neuron.h"

Neuron::Neuron(unsigned weights_num )
{
    if(weights_num == 0)
    {
        m_weights.push_back(1);
        m_d_weights.push_back(0);
    }
    else
    {
        for(unsigned W = 0; W < weights_num; W++)
        {
            m_weights.push_back(randWeight());
            m_d_weights.push_back(0);
        }
    }
}

double Neuron::randWeight(void)
{
    double wyj;
    srand(current_time_nanosec());
    wyj = (((rand() % 1000000L) / 1700.0) - 9.8)*0.0015;
    if(wyj == 0)
        wyj = 0.01492;
    Sleep(1);
    return wyj;

//    srand(current_time_nanosec());
//    Sleep(1);
//    return rand() / double(RAND_MAX);
}

int Neuron::current_time_nanosec(void)
{
    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);
    return tm.tv_nsec;
}

void Neuron::feedForward(LAYER &prevLayer)
{
    double agregate = 0;

    for(unsigned N = 0; N < prevLayer.size(); N++)
    {
        agregate += (prevLayer[N].m_output * m_weights[N]);
    }

    agregate = tranFun(agregate);

    m_output = agregate;
}

void Neuron::calcOutGrad(double Teach)
{
    double delta = Teach - m_output;
    m_gradient = delta * Neuron::transferFunctionDerr(m_output);
}

void Neuron::calcHiddGrad(LAYER &nextLay, unsigned neuron_index)
{
    double gradSum = 0;
    for(unsigned N = 0; N < nextLay.size(); N++)
    {
        gradSum += nextLay[N].m_gradient * nextLay[N].returnWeights()[neuron_index];
    }
    m_gradient = gradSum * Neuron::transferFunctionDerr(m_output);
}

void Neuron::updateWeights(LAYER & prevLay, double eta, double alfa)
{
    for(unsigned W = 0; W < m_weights.size(); W++)
    {
        double oldDelta = m_d_weights[W];
        double newDelta = eta * m_gradient * prevLay[W].m_output + alfa * oldDelta; // prev lay ma tyle neuronów co jest wag w outpucie

        m_d_weights[W] = newDelta;
        m_weights[W] += newDelta;
    }
}
