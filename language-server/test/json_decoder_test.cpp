#include <gtest/gtest.h>

#include "json/decoder.h"
#include "json/element.h"

using namespace winrsls::json;

constexpr const auto JSON = R"({
  "string": "Hello, World!",
  "int": 100,
  "number": 20.5678e+10,
  "bool": true,
  "null": null,
  "array": [1, 2, 3, 4, 5, 6],
  "object": {
    "Message": "Hey!",
    "escape": "\n\t\r\f"
  }
})";

TEST(JsonDecoderTest, Decode) {
  Decoder decoder(JSON);
  const auto decode_result = decoder.decode();
  EXPECT_TRUE(decode_result);
  const auto json = decode_result.get();
  EXPECT_EQ(json.at("string"), String("Hello, World!"));
  EXPECT_EQ(json.at("int"), Number(100));
  EXPECT_EQ(json.at("number"), Number(20.5678e+10));
  EXPECT_EQ(json.at("bool"), Boolean(true));
  EXPECT_EQ(json.at("null"), Null());
  const auto array = Array::from<Number>({1, 2, 3, 4, 5, 6});
  EXPECT_EQ(json.at("array"), array);
  const auto obj = json.at("object").as<Object>();
  EXPECT_EQ(obj.at("Message"), String("Hey!"));
  EXPECT_EQ(obj.at("escape"), String("\n\t\r\f"));
}