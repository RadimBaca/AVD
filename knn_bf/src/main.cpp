#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <omp.h>

struct sfile_info {
    size_t dim;
    size_t count;
    char file_name[256];

    sfile_info(char* p_file_name) {
        strcpy(file_name, p_file_name);
    }
};

//float* fvecs_read(const char* fname, size_t* d_out, size_t* n_out) {
float* fvecs_read(sfile_info& fi)
{
    FILE* f = fopen(fi.file_name, "r");
    if (!f) {
        fprintf(stderr, "could not open %s\n", fi.file_name);
        perror("");
        abort();
    }
    int d;
    fread(&d, 1, sizeof(int), f);
    assert((d > 0 && d < 1000000) || !"unreasonable dimension");
    fseek(f, 0, SEEK_SET);
    struct stat st;
    fstat(fileno(f), &st);
    size_t sz = st.st_size;
    assert(sz % ((d + 1) * 4) == 0 || !"weird file size");
    size_t n = sz / ((d + 1) * 4);

    fi.dim = d;
    fi.count = n;
    float* x = new float[n * (d + 1)];
    size_t nr = fread(x, sizeof(float), n * (d + 1), f);
    assert(nr == n * (d + 1) || !"could not read whole file");

    // shift array to remove row headers
    for (size_t i = 0; i < n; i++)
        memmove(x + i * d, x + 1 + i * (d + 1), d * sizeof(*x));

    fclose(f);
    return x;
}

// not very clean, but works as long as sizeof(int) == sizeof(float)
int* ivecs_read(sfile_info& fi) {
    return (int*)fvecs_read(fi);
}

double distance(float* data, float* query, size_t dim)
{
    double dist = 0;
    for (int k = 0; k < dim; k++)
    {
        double r = data[k] - query[k];
        dist += r * r;
    }
    return dist;
}

void brute_force(float* data, float* query, std::vector<int>& result, size_t dim, uint32_t k, size_t node_count)
{
    std::vector<std::pair<double, int>> nearest;

    // TODO
    // scan the data and compute a distance between each vector and query
    // store K nearest neighbors in the result vector
    for (int i = 0; i < node_count; i++)
    {
        double dist = distance(&data[i * dim], query, dim);
        if (nearest.size() < k)
        {
            nearest.emplace_back(dist, i);
            if (nearest.size() == k) std::sort(nearest.begin(), nearest.end());
        }
        else
        {
            if (dist < nearest[k - 1].first)
            {
                auto pos = std::lower_bound(nearest.begin(), nearest.end(), std::make_pair(dist, i));
                nearest.insert(pos, std::make_pair(dist, i));
                nearest.pop_back();
            }
        }
    }

    // move positions from nearest to result
    for (auto& x : nearest) result.push_back(x.second);
}

void sift_test() {
    int k = 10;
    int queries_processed = 100;

    /////////////////////////////////////////////////////// READ DATA
    sfile_info fi_base("../../sift/sift_base.fvecs");
    sfile_info fi_query("../../sift/sift_query.fvecs");
    sfile_info fi_groundtruth("../../sift/sift_groundtruth.ivecs");

    float* mass = fvecs_read(fi_base);
    float* massQ = fvecs_read(fi_query);
    int* massQA = ivecs_read(fi_groundtruth);

    assert(fi_base.dim == fi_query.dim || !"query does not have same dimension as base set");
    assert(fi_query.count == fi_groundtruth.count || !"incorrect number of ground truth entries");
    assert(queries_processed < fi_query.count || !"not enough queries in the file");
    


    /////////////////////////////////////////////////////// QUERY PART
    std::cout << "Start querying\n";
    std::vector<std::pair<float, float>> precision_time;  


    std::vector<int> result;
    int recall = 0;
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < queries_processed; i++)
    {
        brute_force(mass, &massQ[i * fi_query.dim], result, fi_query.dim, k, fi_base.count);

        // compute recall
        for (auto& r : result) {
            for (int j = 0; j < k; j++) {
                if (r == massQA[i * fi_groundtruth.dim + j]) recall++;
            }
        }
    }
    auto end = std::chrono::steady_clock::now();

    std::cout << "Average query time: " << (float)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / (1000 * queries_processed) << " [ms]\n";
    std::cout << "Recall: " << (double)recall/(k*queries_processed)  << "\n";

    delete[] mass;
    delete[] massQ;
    delete[] massQA;
}



int main(void)
{
    sift_test();

    return 0;
}

