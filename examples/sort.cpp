//
// Created by Ivan Shynkarenka on 18.07.2015
//

#include "benchmark/cppbenchmark.h"

#include <algorithm>
#include <queue>
#include <vector>

const int slow_size = 10000;
const int fast_size = 1000000;

class SortFixture : public virtual CppBenchmark::Fixture
{
protected:
    std::vector<int> items;

    void Initialize(CppBenchmark::Context& context) override
    {
        items.resize(context.x());
    }

    void Cleanup(CppBenchmark::Context& context) override
    {
        items.clear();
    }
};

class SelectionSort : public CppBenchmark::Benchmark, public SortFixture
{
public:
    using Benchmark::Benchmark;

protected:
    void Run(CppBenchmark::Context& context) override
    {
        // Generate items to sort
        std::generate(items.begin(), items.end(), rand);

        // Sort items
        for (size_t i = 0; i < items.size(); ++i)
        {
            // Set the current item as minimal
            size_t min = i;

            // Found in rest items another minimal
            for (size_t j = i + 1; j < items.size(); ++j)
            {
                if (items[j] < items[min])
                    min = j;
            }

            // Swap the current item with minimal
            std::swap(items[i], items[min]);
        }
        context.metrics().AddItems(items.size());
    }
};

class BubbleSort : public CppBenchmark::Benchmark, public SortFixture
{
public:
    using Benchmark::Benchmark;

protected:
    void Run(CppBenchmark::Context& context) override
    {
        // Generate items to sort
        std::generate(items.begin(), items.end(), rand);

        // Iterate through all items to get the current bound
        for (size_t i = 0; i < items.size(); ++i)
        {
            // Calculate the current bound
            size_t bound = items.size() - i;

            // Starting from the current items and for all next items before the bound...
            for (size_t j = 1; j < bound; ++j)
            {
                // Swap items to bubble up minimal one to the end
                if (items[j] < items[j - 1])
                    std::swap(items[j], items[j - 1]);
            }
        }
        context.metrics().AddItems(items.size());
    }
};

class ShakerSort : public CppBenchmark::Benchmark, public SortFixture
{
public:
    using Benchmark::Benchmark;

protected:
    void Run(CppBenchmark::Context& context) override
    {
        // Generate items to sort
        std::generate(items.begin(), items.end(), rand);

        // Start values of left and right bound
        size_t left = 1;
        size_t right = items.size();

        // While left and right bound not met...
        while (left < right)
        {
            // Bubble down minimal item to the left bound
            for (size_t i = right; i > left; --i)
            {
                if (items[i - 1] < items[i - 1 - 1])
                    std::swap(items[i - 1], items[i - 1 - 1]);
            }

            // Increment left bound
            ++left;

            // Bubble up maximal item to the right bound
            for (size_t i = left; i < right; ++i)
            {
                if (items[i + 1 - 1] < items[i - 1])
                    std::swap(items[i + 1 - 1], items[i - 1]);
            }

            // Decrement right bound
            --right;
        }
        context.metrics().AddItems(items.size());
    }
};

class GnomeSort : public CppBenchmark::Benchmark, public SortFixture
{
public:
    using Benchmark::Benchmark;

protected:
    void Run(CppBenchmark::Context& context) override
    {
        // Generate items to sort
        std::generate(items.begin(), items.end(), rand);

        // Current index
        size_t i = 1;
        // Jump index
        size_t j = 2;

        // For each item...
        while (i < items.size())
        {
            // If the current item is less then previous...
            if (items[i] < items[i - 1])
            {
                // Swap items
                std::swap(items[i], items[i - 1]);
                // Decrease the current item
                --i;
                // If we get the left bound then jump the next unsorted item
                if (i == 0)
                {
                    i = j;
                    ++j;
                }
            }
            else
            {
                // In other case jump to the next unsorted item
                i = j;
                ++j;
            }
        }
        context.metrics().AddItems(items.size());
    }
};

class InsertSort : public CppBenchmark::Benchmark, public SortFixture
{
public:
    using Benchmark::Benchmark;

protected:
    void Run(CppBenchmark::Context& context) override
    {
        // Generate items to sort
        std::generate(items.begin(), items.end(), rand);

        // Sort items
        for (size_t i = 1; i < items.size(); ++i)
        {
            // Take the next item
            for (size_t j = i + 1; j > 1; --j)
            {
                // Check the current item with the previous one
                if (items[j - 1] < items[j - 1 - 1])
                    std::swap(items[j - 1], items[j - 1 - 1]);
            }
        }
        context.metrics().AddItems(items.size());
    }
};

class ShellSort : public CppBenchmark::Benchmark, public SortFixture
{
public:
    using Benchmark::Benchmark;

protected:
    void Run(CppBenchmark::Context& context) override
    {
        // Generate items to sort
        std::generate(items.begin(), items.end(), rand);

        // Calculate initial distance
        size_t d = 1;
        while (d < items.size())
            d = 3 * d + 1;

        // While distance is valid
        while (d > 0)
        {
            // Perform insert sort with the given distance
            ShellSortInternal(items, d);

            // Decrease distance
            d /= 3;
        }
        context.metrics().AddItems(items.size());
    }

private:
    void ShellSortInternal(std::vector<int>& subitems, size_t d) const
    {
        for (size_t i = d; i < subitems.size(); ++i)
        {
            // Take the next item
            for (size_t j = i + 1; j >= d + 1; j -= d)
            {
                // Check the current item with the previous one
                if (subitems[j - 1] < subitems[j - d - 1])
                    std::swap(subitems[j - 1], subitems[j - d - 1]);
            }
        }
    }
};

class MergeSort : public CppBenchmark::Benchmark, public SortFixture
{
public:
    using Benchmark::Benchmark;

protected:
    void Run(CppBenchmark::Context& context) override
    {
        // Generate items to sort
        std::generate(items.begin(), items.end(), rand);

        // Use temporary array
        std::vector<int> temp(items.size());

        // Start from the chunk size 1
        size_t chunk = 1;
        // While chunk size less than count of items
        while (chunk < items.size())
        {
            // Perform merge operation for the current chunks
            for (size_t i = 0; i < items.size(); i += 2 * chunk)
                MergeSortInternal(temp.data() + i, items.data() + i, i, items.size(), chunk);
            // Increase chunk size
            chunk *= 2;
            // Swap arrays
            std::swap(temp, items);
        }
        context.metrics().AddItems(items.size());
    }

private:
    static void MergeSortInternal(int* dst, int* src, size_t index, size_t size, size_t chunk)
    {
        size_t index1 = 0;
        size_t index2 = 0;

        while ((index1 < chunk) || (index2 < chunk))
        {
            // Check bounds of left chunk
            if ((index + index1 >= size))
                index1 = chunk;
            // Check bounds of right chunk
            if (index + chunk + index2 >= size)
                index2 = chunk;

            // Check if we use right or left chunk for merge
            if ((index2 < chunk) && ((index1 == chunk) || (src[chunk + index2] < src[index1])))
            {
                // Use right chunk
                *dst++ = src[chunk + index2];
                ++index2;
            }
            else if (index1 < chunk)
            {
                // Use left chunk
                *dst++ = src[index1];
                ++index1;
            }
        }
    }
};

class QuickSort : public CppBenchmark::Benchmark, public SortFixture
{
public:
    using Benchmark::Benchmark;

protected:
    void Run(CppBenchmark::Context& context) override
    {
        // Generate items to sort
        std::generate(items.begin(), items.end(), rand);

        // Sort items
        if (items.size() > 0)
            QuickSortInternal(items, 1, items.size());
        context.metrics().AddItems(items.size());
    }

private:
    static void QuickSortInternal(std::vector<int>& subitems, size_t left, size_t right)
    {
        // Choose the pivot item
        int pivot = subitems[left + ((right - left) / 2) - 1];

        size_t l = left;
        size_t r = right;

        while (l <= r)
        {
            // Move left if item is less than pivot
            while ((l < right) && (subitems[l - 1] < pivot))
                l++;
            // Move right if item is less than pivot
            while ((r > left) && (subitems[r - 1] > pivot))
                r--;

            if (l <= r)
            {
                // Swap left and right items
                std::swap(subitems[l - 1], subitems[r - 1]);

                // Move left and right indexes
                ++l;
                --r;
            }
        }

        // Perform quick sort operation for the left sub items
        if (left < r)
            QuickSortInternal(subitems, left, r);
        // Perform quick sort operation for the right sub items
        if (l < right)
            QuickSortInternal(subitems, l, right);
    }
};

class QuickSort3 : public CppBenchmark::Benchmark, public SortFixture
{
public:
    using Benchmark::Benchmark;

protected:
    void Run(CppBenchmark::Context& context) override
    {
        // Generate items to sort
        std::generate(items.begin(), items.end(), rand);

        // Sort items
        if (items.size() > 0)
            QuickSort3Internal(items, 1, items.size());
        context.metrics().AddItems(items.size());
    }

private:
    static void QuickSort3Internal(std::vector<int>& subitems, size_t left, size_t right)
    {
        // Choose the pivot item
        int pivot = subitems[left + ((right - left) / 2) - 1];

        size_t bl = left;
        size_t br = right;

        size_t l = left;
        size_t r = right;

        while (l <= r)
        {
            // Move left if item is less than pivot
            while ((l < right) && (subitems[l - 1] < pivot))
                l++;
            // Move right if item is less than pivot
            while ((r > left) && (subitems[r - 1] > pivot))
                r--;

            if (l <= r)
            {
                // Swap left and right items
                std::swap(subitems[l - 1], subitems[r - 1]);

                // Move equals to the left
                if (subitems[l - 1] == pivot)
                {
                    std::swap(subitems[bl - 1], subitems[l - 1]);
                    bl++;
                }

                // Move equals to the right
                if (subitems[r - 1] == pivot)
                {
                    std::swap(subitems[br - 1], subitems[r - 1]);
                    br++;
                }

                // Move left and right indexes
                ++l;
                --r;
            }
        }

        // Swap left equals with the lower items
        for (size_t k = left; (k < bl) && (r > left); ++k, --r)
            std::swap(subitems[k - 1], subitems[r - 1]);
        // Swap right equals with the upper items
        for (size_t k = right; (k > br) && (l < right); --k, ++l)
            std::swap(subitems[k - 1], subitems[l - 1]);

        // Perform quick sort operation for the left sub items
        if (left < r)
            QuickSort3Internal(subitems, left, r);
        // Perform quick sort operation for the right sub items
        if (l < right)
            QuickSort3Internal(subitems, l, right);
    }
};

class RadixSort : public CppBenchmark::Benchmark, public SortFixture
{
public:
    using Benchmark::Benchmark;

protected:
    void Run(CppBenchmark::Context& context) override
    {
        // Generate items to sort
        std::generate(items.begin(), items.end(), rand);

        // Cache radix queue for all digits
        std::queue<int> radix_queue[10];

        // Initial radix base
        size_t n = 1;

        bool found;
        do
        {
            // Reset found flag
            found = false;

            // Try to put all items into the radix queue
            for (size_t i = 0; i < items.size(); ++i)
            {
                // Get the base item value
                size_t base = items[i] / n;

                // Update found flag
                found |= base > 0;

                // Get the item index in the radix queue
                size_t index = base % 10;

                // Add the item into the radix queue
                radix_queue[index].push(items[i]);
            }

            // Back items from the radix list
            if (found)
            {
                size_t index = 0;
                for (size_t i = 0; i < sizeof(radix_queue) / sizeof(radix_queue[0]); ++i)
                {
                    // Get all items from the corresponding radix queue
                    while (!radix_queue[i].empty())
                    {
                        items[index++] = radix_queue[i].front();
                        radix_queue[i].pop();
                    }
                }
            }

            // Increase the current radix base
            n *= 10;
        } while (found);
        context.metrics().AddItems(items.size());
    }
};

class StdSort : public CppBenchmark::Benchmark, public SortFixture
{
public:
    using Benchmark::Benchmark;

protected:
    void Run(CppBenchmark::Context& context) override
    {
        // Generate items to sort
        std::generate(items.begin(), items.end(), rand);

        // Sort items
        std::sort(items.begin(), items.end());
        context.metrics().AddItems(items.size());
    }
};

BENCHMARK_CLASS(SelectionSort, "SelectionSort", Settings().Param(slow_size))
BENCHMARK_CLASS(BubbleSort, "BubbleSort", Settings().Param(slow_size))
BENCHMARK_CLASS(ShakerSort, "ShakerSort", Settings().Param(slow_size))
BENCHMARK_CLASS(GnomeSort, "GnomeSort", Settings().Param(slow_size))
BENCHMARK_CLASS(InsertSort, "InsertSort", Settings().Param(slow_size))
BENCHMARK_CLASS(ShellSort, "ShellSort", Settings().Param(slow_size))
BENCHMARK_CLASS(MergeSort, "MergeSort", Settings().Param(fast_size))
BENCHMARK_CLASS(QuickSort, "QuickSort", Settings().Param(fast_size))
BENCHMARK_CLASS(QuickSort3, "QuickSort3", Settings().Param(fast_size))
BENCHMARK_CLASS(RadixSort, "RadixSort", Settings().Param(fast_size))
BENCHMARK_CLASS(StdSort, "std::sort", Settings().Param(fast_size))

BENCHMARK_MAIN()
