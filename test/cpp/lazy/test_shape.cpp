#include <gtest/gtest.h>

#include <sstream>

#include <torch/csrc/lazy/core/shape.h>

namespace torch {
namespace lazy {

TEST(ShapeTest, Basic1) {
  auto shape = Shape();

  EXPECT_STREQ(shape.to_string().c_str(), "UNKNOWN_SCALAR[]");
  EXPECT_EQ(shape.scalar_type(), c10::ScalarType::Undefined);
  EXPECT_EQ(shape.dim(), 0);
  EXPECT_TRUE(shape.sizes().empty());
  EXPECT_THROW(shape.size(0), std::out_of_range);
}

TEST(ShapeTest, Basic2) {
  auto shape = Shape(c10::ScalarType::Float, {1, 2, 3});

  EXPECT_STREQ(shape.to_string().c_str(), "Float[1,2,3]");
  EXPECT_EQ(shape.scalar_type(), c10::ScalarType::Float);
  EXPECT_EQ(shape.dim(), 3);
  EXPECT_EQ(shape.sizes().size(), 3);
  for (int64_t i = 0; i < shape.dim(); i++) {
    EXPECT_EQ(shape.sizes()[i], i + 1);
    EXPECT_EQ(shape.size(i), i + 1);
  }
}

TEST(ShapeTest, Basic3) {
  auto shape = Shape(c10::ScalarType::Float, {});

  EXPECT_STREQ(shape.to_string().c_str(), "Float[]");
  EXPECT_EQ(shape.scalar_type(), c10::ScalarType::Float);
  EXPECT_EQ(shape.dim(), 0);
  EXPECT_TRUE(shape.sizes().empty());
  EXPECT_THROW(shape.size(0), std::out_of_range);
}

TEST(ShapeTest, SetScalarType) {
  auto shape = Shape();

  shape.set_scalar_type(c10::ScalarType::Long);
  EXPECT_EQ(shape.scalar_type(), c10::ScalarType::Long);
}

TEST(ShapeTest, SetSize) {
  auto shape1 = Shape();
  EXPECT_THROW(shape1.set_size(0, 0), std::out_of_range);

  auto shape2 = Shape(c10::ScalarType::Float, {1, 2, 3});
  shape2.set_size(0, 3);
  EXPECT_EQ(shape2.sizes()[0], 3);
  EXPECT_EQ(shape2.size(0), 3);
}

TEST(ShapeTest, Equal) {
  auto shape1 = Shape(c10::ScalarType::Float, {});
  auto shape2 = Shape(c10::ScalarType::Float, {1, 2, 3});
  auto shape3 = Shape(c10::ScalarType::Long, {1, 2, 3});
  auto shape4 = Shape(c10::ScalarType::Float, {1, 2, 3});

  EXPECT_FALSE(shape1 == shape2);
  EXPECT_FALSE(shape2 == shape3);
  EXPECT_FALSE(shape1 == shape3);
  EXPECT_TRUE(shape2 == shape2);
}

TEST(ShapeTest, Ostream) {
  auto shape = Shape();
  std::stringstream ss;
  ss << shape;

  EXPECT_EQ(shape.to_string(), ss.str());
}

TEST(ShapeTest, ConvertShapes) {
  auto shape1 = Shape(c10::ScalarType::Long, {1, 2, 3});
  auto shape2 = Shape(c10::ScalarType::Float, {1, 2});

  auto shapes1 = convertShapes({}, {});
  EXPECT_TRUE(shapes1.empty());

  auto shapes2 = convertShapes({c10::ScalarType::Long}, {{1, 2, 3}});
  EXPECT_EQ(shapes2.size(), 1);
  EXPECT_EQ(shapes2[0], shape1);

  auto shapes3 = convertShapes({c10::ScalarType::Long, c10::ScalarType::Float}, {{1, 2, 3}, {1, 2}});
  EXPECT_EQ(shapes3.size(), 2);
  EXPECT_EQ(shapes3[0], shape1);
  EXPECT_EQ(shapes3[1], shape2);
}

}  // namespace lazy
}  // namespace torch
