/*!
 * Copyright 2015 by Contributors
 * \file simple_dmatrix.h
 * \brief In-memory version of DMatrix.
 * \author Tianqi Chen
 */
#ifndef XGBOOST_DATA_SIMPLE_DMATRIX_H_
#define XGBOOST_DATA_SIMPLE_DMATRIX_H_

#include <xgboost/base.h>
#include <xgboost/data.h>

#include <memory>
#include <string>


namespace xgboost {
namespace data {
// Used for single batch data.
class SimpleDMatrix : public DMatrix {
 public:
  template <typename AdapterT>
  explicit SimpleDMatrix(AdapterT* adapter, float missing, int nthread);

  explicit SimpleDMatrix(dmlc::Stream* in_stream);

  void SaveToLocalFile(const std::string& fname);

  MetaInfo& Info() override;

  const MetaInfo& Info() const override;

  float GetColDensity(size_t cidx) override;

  bool SingleColBlock() const override;

  /*! \brief magic number used to identify SimpleDMatrix binary files */
  static const int kMagic = 0xffffab01;

 private:
  BatchSet<SparsePage> GetRowBatches() override;
  BatchSet<CSCPage> GetColumnBatches() override;
  BatchSet<SortedCSCPage> GetSortedColumnBatches() override;
  BatchSet<EllpackPage> GetEllpackBatches(const BatchParam& param) override;

  MetaInfo info;
  SparsePage sparse_page_;  // Primary storage type
  std::unique_ptr<CSCPage> column_page_;
  std::unique_ptr<SortedCSCPage> sorted_column_page_;
  std::unique_ptr<EllpackPage> ellpack_page_;
};
}  // namespace data
}  // namespace xgboost
#endif  // XGBOOST_DATA_SIMPLE_DMATRIX_H_
