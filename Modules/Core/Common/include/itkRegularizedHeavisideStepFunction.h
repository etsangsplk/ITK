/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkRegularizedHeavisideStepFunction_h
#define __itkRegularizedHeavisideStepFunction_h

#include "itkHeavisideStepFunctionBase.h"

namespace itk
{
/** \class RegularizedHeavisideStepFunction
 *
 * \brief Base class of the Regularized (smoothed) Heaviside functions.
 *
 * \author Mosaliganti K., Smith B., Gelas A., Gouaillard A., Megason S.
 *
 *  This code was taken from the Insight Journal paper:
 *
 *      "Cell Tracking using Coupled Active Surfaces for Nuclei and Membranes"
 *      http://www.insight-journal.org/browse/publication/642
 *      http://hdl.handle.net/10380/3055
 *
 *  That is based on the papers:
 *
 *      "Level Set Segmentation: Active Contours without edge"
 *      http://www.insight-journal.org/browse/publication/322
 *      http://hdl.handle.net/1926/1532
 *
 *      and
 *
 *      "Level set segmentation using coupled active surfaces"
 *      http://www.insight-journal.org/browse/publication/323
 *      http://hdl.handle.net/1926/1533
 *
 *
 * \ingroup ITKCommon
 */
template< typename TInput = float, typename TOutput = double >
class RegularizedHeavisideStepFunction:
    public HeavisideStepFunctionBase< TInput, TOutput >
{
public:
  typedef RegularizedHeavisideStepFunction             Self;
  typedef HeavisideStepFunctionBase< TInput, TOutput > Superclass;
  typedef SmartPointer< Self >                         Pointer;
  typedef SmartPointer< const Self >                   ConstPointer;

  typedef typename Superclass::InputType  InputType;
  typedef typename Superclass::OutputType OutputType;

  typedef typename NumericTraits< InputType >::RealType RealType;

  void SetEpsilon(const RealType & ieps);

  itkGetConstMacro( Epsilon, RealType );
  itkGetConstMacro( OneOverEpsilon, RealType );

protected:
  RegularizedHeavisideStepFunction();
  virtual ~RegularizedHeavisideStepFunction();

private:
  RegularizedHeavisideStepFunction(const Self &); //purposely not implemented
  void operator=(const Self &);                   //purposely not implemented

  RealType m_Epsilon;
  RealType m_OneOverEpsilon;
};
}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkRegularizedHeavisideStepFunction.hxx"
#endif

#endif
