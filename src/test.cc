#include <memory>
#include <mutex>

#include "object_pool.h"

class AIPredict {
 public:
  AIPredict() {}
  ~AIPredict() {}
  int Init(std::string model_path) { std::cout << "init" << std::endl; }
  int Predict(std::string img_buf, std::vector<float>* feat) {
    feat->clear();
    for (int i = 0; i < feature_len_; i++) {
      feat->push_back((float)std::rand() / RAND_MAX);
    }
    return 0;
  }

 private:
  int feature_len_{10};
};

int main() {
  ObjectPool<AIPredict> object_pool_;
  int engine_num = 5;
  for (int i = 0; i < engine_num; i++) {
    std::unique_ptr<AIPredict> engine(new AIPredict());
    int ret = engine->Init("./model");
    object_pool_.Push(std::move(engine));
  }

  auto predict = object_pool_.Get(1000);
  std::vector<float> feature;
  predict->Predict("xxxxx", &feature);
  for (int i = 0; i < feature.size(); i++) {
    if (i == feature.size() - 1) {
      std::cout << feature[i];
    } else {
      std::cout << feature[i] << ",";
    }
  }
  std::cout << std::endl;
}