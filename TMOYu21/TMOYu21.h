/* --------------------------------------------------------------------------- *
 * TMOYu21.h														  		   *
 *																			   *
 * Author: Ludmila Krejčová													   *
 * --------------------------------------------------------------------------- */


#include "TMO.h"
#include <vector>
#include <memory>

class TMOYu21 : public TMO
{
public:
	TMOYu21();
	virtual ~TMOYu21();
	virtual int Transform();
protected:

	std::array<double, 3> computeK(std::array<double, 3> &Krg_Kgb_Kbr, cv::Mat imgR, cv::Mat imgG, cv::Mat imgB);

	std::unique_ptr<std::vector<double>>  createContrastImage(std::array<double, 3> &Krg_Kgb_Kbr);

	std::array<double, 3> computeKrg_Kgb_Kbr();

	std::array<double, 3> computeSSIM(std::unique_ptr<std::vector<double>> &contrastImage, cv::Mat imgR, cv::Mat imgG, cv::Mat imgB);

	inline double getPixel(const double* data, int width, int x, int y, int channel);
	inline void setPixel(double* data, int width, int x, int y, int channel, double value);

	std::unique_ptr<double[]> resizeImage(const double* input, int srcWidth, int srcHeight, int destWidth, int destHeight);

	std::shared_ptr<std::vector<double>> computeContrastDifferences(const std::vector<std::pair<int, int>> &pairs, const double* image64, 
				const double* image32, int channel);
	
	std::array<double, 3> computeWeights(const std::vector<double> &allIr, const std::vector<double> &allIg,
				const std::vector<double> &allIb, const std::array<double, 3> &kr_kg_kb, double epsilon);

	double 	computeColorEnergy(const std::array<double, 3> &w, double k, const std::array<std::vector<double>, 3> &I, size_t colorIndex, double epsilon);

	std::vector<std::pair<int, int>> findRandomPairs(int size) const;

protected:
	TMODouble dParameter;
};
