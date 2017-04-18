/*
  Copyright ©2013 The Regents of the University of California
  (Regents). All Rights Reserved. Permission to use, copy, modify, and
  distribute this software and its documentation for educational,
  research, and not-for-profit purposes, without fee and without a
  signed licensing agreement, is hereby granted, provided that the
  above copyright notice, this paragraph and the following two
  paragraphs appear in all copies, modifications, and
  distributions. Contact The Office of Technology Licensing, UC
  Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley, CA 94720-1620,
  (510) 643-7201, for commercial licensing opportunities.

  IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT,
  INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
  LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
  DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY
  OF SUCH DAMAGE.

  REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
  DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS
  IS". REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
  UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/
#pragma once

#include "bvh.hpp"

namespace ldp
{
	class LevelSet3D;
}

namespace arcsim
{

	typedef DeformBVHNode BVHNode;
	typedef DeformBVHTree BVHTree;

	struct AccelStruct
	{
		BVHTree tree;
		BVHNode *root;
		std::vector<BVHNode*> leaves;
		AccelStruct(const Mesh &mesh, bool ccd);
	};

	void update_accel_struct(AccelStruct &acc);

	void mark_all_inactive(AccelStruct &acc);
	void mark_active(AccelStruct &acc, const Face *face);

	// callback must be safe to parallelize via OpenMP
	typedef void(*BVHCallback) (const Face *face0, const Face *face1);
	typedef void(*BVHCallback_1) (const Face *face0, const ldp::LevelSet3D *face1);

	void for_overlapping_faces(BVHNode *node, float thickness,
		BVHCallback callback);
	void for_overlapping_faces(BVHNode *node0, BVHNode *node1, float thickness,
		BVHCallback callback);
	void for_overlapping_faces(const std::vector<AccelStruct*> &accs,
		const std::vector<AccelStruct*> &obs_accs,
		double thickness, BVHCallback callback,
		bool parallel = true);
	void for_faces_overlapping_obstacles(const std::vector<AccelStruct*> &accs,
		const std::vector<AccelStruct*> &obs_accs,
		double thickness, BVHCallback callback,
		bool parallel = true);

	/// ldp: dist field based object-cloth collision
	void for_overlapping_faces(BVHNode *node, float thickness,
		BVHCallback_1 callback);
	void for_overlapping_faces(BVHNode *node0, ldp::LevelSet3D* node1, float thickness,
		BVHCallback_1 callback);
	void for_overlapping_faces(const std::vector<AccelStruct*> &accs,
		const std::vector<ldp::LevelSet3D*> &obs_accs,
		double thickness, BVHCallback callback, BVHCallback_1 callback1,
		bool parallel = true);
	void for_faces_overlapping_obstacles(const std::vector<AccelStruct*> &accs,
		const std::vector<ldp::LevelSet3D*> &obs_accs,
		double thickness, BVHCallback_1 callback,
		bool parallel = true);

	std::vector<AccelStruct*> create_accel_structs
		(const std::vector<Mesh*> &meshes, bool ccd);
	void destroy_accel_structs(std::vector<AccelStruct*> &accs);

	// find index of mesh containing specified element
	template <typename Prim>
	int find_mesh(const Prim *p, const std::vector<Mesh*> &meshes);

	extern const std::vector<Mesh*> *meshes; // to check if element is cloth or obs
	extern const std::vector<Mesh*> *obs_meshes;
	template <typename Primitive> bool is_free(const Primitive *p)
	{
		return find_mesh(p, *arcsim::meshes) != -1;
	}

}