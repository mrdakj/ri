// output image dimensions are the same as the input image dimensions
img::Image<img::Type::GRAYSCALE> output(rows(), cols());

unsigned min_gray = *std::min_element((*this).cbegin(), (*this).cend());
unsigned max_gray = *std::max_element((*this).cbegin(), (*this).cend());
unsigned diff = max_gray - min_gray;

for (int i = 0; i < rows(); i++) {
    for (int j = 0; j <  cols(); ++j) {
        // fuzzification
        double mem_func = ((*this)(i, j) - min_gray) / double(diff);
        
        // modification of membership function by INT operator
        if (mem_func <= 0.5 and mem_func > 0) {
            mem_func = 2 * pow(mem_func, 2);
        }
        else if (mem_func <= 1 and mem_func > 0.5) {
            mem_func = 1 - 2 * pow(1 - mem_func, 2);
        }

        // defuzzification
        output(i, j) = min_gray + mem_func * diff; 
    }
}
    
return output;
