// ============================================================================
//
// = CONTEXT
//   SALLV Generic Client for LabVIEW
//
// = FILENAME
//   GroupDescriptor.i
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL : SAL adaptation : Dave Mills - LSST
//
// ============================================================================

// ============================================================================
//
// ============================================================================
LV_INLINE SALLV::Group * GroupDescriptor::operator-> ()
{
  return g;
}

// ============================================================================
//
// ============================================================================
LV_INLINE SALLV::Group * GroupDescriptor::group ()
{
  return g;
}

// ============================================================================
//
// ============================================================================
LV_INLINE const SubsysList& GroupDescriptor::Subsys_list ()
{
  return dl;
}

// ============================================================================
//
// ============================================================================
LV_INLINE bool GroupDescriptor::operator< (const GroupDescriptor& _src) const
{
  return (long)g < (long)_src.g;
}



