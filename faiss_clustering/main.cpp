#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <faiss/Clustering.h>
#include <faiss/IndexFlat.h>
#include <faiss/utils/utils.h>


using namespace std;

struct sfile_info {
    size_t dim;
    size_t count;
    char file_name[256];

    sfile_info(char* p_file_name) {
        strcpy(file_name, p_file_name);
    }
};

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


int main()
{
    int numberOfEMIterations = 20;
    size_t numberOfClusters = 1000;

    /////////////////////////////////////////////////////// READ DATA
    sfile_info fi_base("../../sift/sift_base.fvecs");
    sfile_info fi_learn("../../sift/sift_learn.fvecs");
    sfile_info fi_query("../../sift/sift_query.fvecs");
    sfile_info fi_groundtruth("../../sift/sift_groundtruth.ivecs");

    float* mass = fvecs_read(fi_base);
    float* massL = fvecs_read(fi_learn);
    float* massQ = fvecs_read(fi_query);
    int* massQA = ivecs_read(fi_groundtruth);

    assert(fi_base.dim == fi_query.dim || !"query does not have same dimension as base set");
    assert(fi_base.dim == fi_learn.dim || !"learn does not have same dimension as base set");
    assert(fi_query.count == fi_groundtruth.count || !"incorrect number of ground truth entries");


    /////////////////////////////////////////////////////// PROCESS CLUSTERING

    faiss::ClusteringParameters cp;
    cp.niter = numberOfEMIterations;
    cp.verbose = true; // print out per-iteration stats

    faiss::IndexFlatL2 index(fi_learn.dim);
    faiss::Clustering kMeans(fi_learn.dim, numberOfClusters, cp);
    kMeans.train(fi_learn.count, massL, index);

    // Print first ten centroids
    int print_center_count = 10;
    for (int c = 0; c < numberOfClusters; c++)
    {
        if (print_center_count-- == 0) break;
        for (int d = 0; d < fi_learn.dim; d++) {
            std::cout << kMeans.centroids[c * fi_learn.dim + d] << " ";    
        }
        std::cout << c << "\n";
    }
  
}