/* The copyright in this software is being made available under the BSD
 * Licence, included below.  This software may be subject to other third
 * party and contributor rights, including patent rights, and no such
 * rights are granted under this licence.
 *
 * Copyright (c) 2017-2018, ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of the ISO/IEC nor the names of its contributors
 *   may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <functional>
#include <map>

#include "PayloadBuffer.h"
#include "PCCMath.h"
#include "PCCPointSet.h"
#include "hls.h"

namespace pcc {

//============================================================================

struct DecoderParams {
  // Do not delete this structure -- it is for passing options to the decoder.
};

//============================================================================

class PCCTMC3Decoder3 {
public:
  PCCTMC3Decoder3() { init(); }
  PCCTMC3Decoder3(const PCCTMC3Decoder3&) = default;
  PCCTMC3Decoder3& operator=(const PCCTMC3Decoder3& rhs) = default;
  ~PCCTMC3Decoder3() = default;

  void init();

  int decompress(
    const DecoderParams& params,
    const PayloadBuffer* buf,
    std::function<void(const PCCPointSet3&)> onOutputCloud);

  //==========================================================================

  void storeSps(SequenceParameterSet&& sps);
  void storeGps(GeometryParameterSet&& gps);
  void storeAps(AttributeParameterSet&& aps);
  void storeTileInventory(TileInventory&& inventory);

  //==========================================================================

private:
  int decodeGeometryBrick(const PayloadBuffer& buf);
  void decodeAttributeBrick(const PayloadBuffer& buf);

  //==========================================================================

public:
  void inverseQuantization(PCCPointSet3& pointCloud);

  //==========================================================================

private:
  // Current identifier of payloads with the same geometry
  int _sliceId;

  // Position of the slice in the translated+scaled co-ordinate system.
  PCCVector3<int> _sliceOrigin;

  // The point cloud currently being decoded
  PCCPointSet3 _currentPointCloud;
  PCCPointSet3 _accumCloud;

  // Received parameter sets, mapping parameter set id -> parameterset
  std::map<int, SequenceParameterSet> _spss;
  std::map<int, GeometryParameterSet> _gpss;
  std::map<int, AttributeParameterSet> _apss;

  // Metadata that allows slices/tiles to be indentified by their bounding box
  TileInventory _tileInventory;

  // The active SPS
  const SequenceParameterSet* _sps;
  const GeometryParameterSet* _gps;
};

//============================================================================

}  // namespace pcc
