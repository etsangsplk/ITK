/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkDiscreteGaussianDerivativeImageFunctionTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkDiscreteGaussianDerivativeImageFunction.h"
#include "itkRescaleIntensityImageFilter.h"

template < int VDimension >
int itkDiscreteGaussianDerivativeImageFunctionTestND( int argc, char* argv[] )
{

  // Verify the number of parameters in the command line
  if( argc < 5 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "inputFileName outputFileName order sigma (maximum_error) (maximum_kernel_width)" << std::endl;
    return EXIT_FAILURE;
    }

  // Define the dimension of the images
  const unsigned int Dimension = VDimension;
  typedef float                            PixelType;
  typedef itk::Image<PixelType, Dimension> ImageType;

  // Read input
  typedef itk::ImageFileReader< ImageType > ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  try
    {
    reader->Update();
    }
  catch ( itk::ExceptionObject &err)
    {
    std::cout << "ExceptionObject caught !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    }

  // Create image for storing result
  typename ImageType::Pointer output = ImageType::New();
  output->SetSpacing( reader->GetOutput()->GetSpacing() );
  output->SetOrigin( reader->GetOutput()->GetOrigin() );
  output->SetDirection( reader->GetOutput()->GetDirection() );
  output->SetLargestPossibleRegion( reader->GetOutput()->GetLargestPossibleRegion() );
  output->SetRequestedRegion( reader->GetOutput()->GetRequestedRegion() );
  output->SetBufferedRegion( reader->GetOutput()->GetBufferedRegion() );
  output->Allocate();
  output->FillBuffer( itk::NumericTraits<PixelType>::Zero );

  // Setup operator parameters
  unsigned int order[Dimension];
  for (unsigned int i = 0; i < Dimension; i++)
    {
    order[i] = atoi( argv[3] );
    }

  double variance = atof( argv[4] );
  variance *= variance;

  double maxError = 0.001;
  unsigned int maxKernelWidth = 100;
  if( argc == 6 )
    {
    maxError = atof( argv[5] );
    }
  else if( argc > 6 )
    {
    maxError = atof( argv[5] );
    maxKernelWidth = atoi( argv[6] );
    }

  // Create function
  typedef itk::DiscreteGaussianDerivativeImageFunction< ImageType, PixelType >
    GaussianDerivativeImageFunctionType;
  typename GaussianDerivativeImageFunctionType::Pointer function = 
    GaussianDerivativeImageFunctionType::New();
  function->SetInputImage( reader->GetOutput() );
  function->SetMaximumError( maxError );
  function->SetMaximumKernelWidth( maxKernelWidth );
  function->SetVariance( variance );
  function->SetOrder( order );
  function->SetNormalizeAcrossScale( true );
  function->SetUseImageSpacing( true );
  function->Initialize( );

  // Step over input and output images
  typedef itk::ImageRegionConstIterator< ImageType > ConstIteratorType;
  typedef itk::ImageRegionIterator< ImageType >      IteratorType;

  ConstIteratorType it ( reader->GetOutput(), reader->GetOutput()->GetRequestedRegion() );
  it.GoToBegin();
  IteratorType out( output, output->GetRequestedRegion() );
  out.GoToBegin();

  while( !it.IsAtEnd() )
    {
    out.Set( function->EvaluateAtIndex(it.GetIndex()) );
    ++it;
    ++out;
    }

  // Rescale output
  typedef unsigned char                                                   OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension >                        OutputImageType;
  typedef itk::RescaleIntensityImageFilter< ImageType, OutputImageType >  RescaleType;

  typename RescaleType::Pointer rescaler = RescaleType::New();
  rescaler->SetInput( output );
  rescaler->SetOutputMinimum( itk::NumericTraits<OutputPixelType>::min() );
  rescaler->SetOutputMaximum( itk::NumericTraits<OutputPixelType>::max() );

  // Write output
  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
  writer->SetInput( rescaler->GetOutput() );
  try
    {
    writer->Update();
    }
  catch ( itk::ExceptionObject &err)
    {
    std::cout << "ExceptionObject caught !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int itkDiscreteGaussianDerivativeImageFunctionTest(int argc, char* argv[] )
{
  return itkDiscreteGaussianDerivativeImageFunctionTestND< 2 >( argc, argv );
}
