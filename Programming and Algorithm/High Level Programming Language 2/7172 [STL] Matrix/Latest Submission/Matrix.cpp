#include <iostream>
using namespace std;
#include "Matrix.hpp"
#include <utility>

void Matrix::resize(int nrow,int ncol){
	if(!nrow || !ncol) return ;
	this->data.resize(nrow);
	for(int i = 0;i < this->data.size();i++)
		this->data[i].resize(ncol);
}
pair<int ,int > Matrix::size() const { 
	if(!this->data.size()) return make_pair(0,0);
	else return make_pair(this->data.size(),this->data[0].size()); 
}
vector<int>& Matrix::operator [] (int r) throw(out_of_range) {
	if(r >= this->data.size()) throw out_of_range("F");
	else return this->data[r];
} 
const vector<int>& Matrix::operator [] (int r) const throw(out_of_range) {
	if(r >= this->data.size()) throw out_of_range("F");
	else return this->data[r];
}
Matrix Matrix::operator + (const Matrix& other) throw(invalid_argument) {
	Matrix result; result.data.resize(0);
	if(!this->data.size() || !other.data.size()) return result;
	if(this->data.size() != other.data.size() || this->data[0].size() != other.data[0].size()) throw invalid_argument("F");
	result.resize(this->data.size(), this->data[0].size());
	for(int i = 0;i < result.data.size();i++)
		for(int j = 0;j < result.data[0].size();j++)
			result.data[i][j] = this->data[i][j] + other.data[i][j];
	return result;
}
Matrix Matrix::operator - (const Matrix& other) throw(invalid_argument) {
	Matrix result; result.data.resize(0);
	if(!this->data.size() || !other.data.size()) return result;
	if(this->data.size() != other.data.size() || this->data[0].size() != other.data[0].size()) throw invalid_argument("F");
	result.resize(this->data.size(), this->data[0].size());
	for(int i = 0;i < result.data.size();i++)
		for(int j = 0;j < result.data[0].size();j++)
			result.data[i][j] = this->data[i][j] - other.data[i][j];
	return result;
}
Matrix Matrix::dotProduct(const Matrix& other) throw(invalid_argument) {
	Matrix result; result.data.resize(0);
	if(!this->data.size() || !other.data.size()) return result;
	if(this->data.size() != other.data.size() || this->data[0].size() != other.data[0].size()) throw invalid_argument("F");
	result.resize(this->data.size(), this->data[0].size());
	for(int i = 0;i < result.data.size();i++)
		for(int j = 0;j < result.data[0].size();j++)
			result.data[i][j] = this->data[i][j] * other.data[i][j];
	return result;
}
Matrix Matrix::innerProduct(const Matrix& other) throw(invalid_argument) {
	Matrix result; result.data.resize(0);
	if(!this->data.size() || !other.data.size()) return result;
	if(this->data[0].size() != other.data.size()) throw invalid_argument("F");
	result.resize(this->data.size(), other.data[0].size());
	for(int i = 0;i < this->data.size();i++)
		for(int j = 0;j < other.data[0].size();j++)
			for(int k = 0;k < this->data[0].size();k++)
				result.data[i][j] += this->data[i][k] * other.data[k][j];
	return result;
}
Matrix Matrix::operator * (const Matrix& other) throw(invalid_argument) {
	Matrix result; result.data.resize(0);
	if(!this->data.size() || !other.data.size()) return result;
	if(this->data[0].size() != other.data.size()) throw invalid_argument("F");
	return this->innerProduct(other);
}
void Matrix::print() {
	if(!this->data.size()) return ;
//	cout<<"J"<<endl;
	for(int i = 0;i < this->data.size();i++)
		for(int j = 0;j < this->data[0].size();j++)
			if(j == this->data[0].size() - 1 ) cout<<this->data[i][j]<<endl;
			else cout<<this->data[i][j]<<' ';
}
