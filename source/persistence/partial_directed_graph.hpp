// Copyright 2021 Roger Peralta Aranibar
#ifndef SOURCE_PERSISTENCE_PARTIAL_DIRECTED_GRAPH_HPP_
#define SOURCE_PERSISTENCE_PARTIAL_DIRECTED_GRAPH_HPP_

#include <utility>
#include <vector>

namespace ADE {
	namespace Persistence {

		template <typename Type>
		class PartialNode {
		public:
			typedef Type data_type;

			PartialNode() : data_(nullptr), forward_(nullptr), backward_(nullptr), out_ptrs_size_(0), in_ptrs_size_(0) {}

			PartialNode(data_type const& data, std::size_t const& out_ptrs_size,
				std::size_t const& in_ptrs_size) {

				backward_ = new PartialNode<Type> * [in_ptrs_size]();
				forward_ = new PartialNode<Type> * [out_ptrs_size]();
				*data_ = data;
				in_ptrs_size_ = in_ptrs_size;
				out_ptrs_size_ = out_ptrs_size;
			}

			data_type get_data(unsigned int version) { 
				return *(this[version].data_); 
			}

			bool is_table_full() {
				for (int i = 0; i < table.size(); i++) {
					if (table[i] == nullptr) {
						return false;
					}
				}
				return true;
			}

			bool set_data(data_type const& data) {
				PartialNode* new_version = new PartialNode<Type>(data, out_ptrs_size_, in_ptrs_size_);

				if (is_table_full()) {
					insert_vertex(next_position; data);
					return true;
				}

				int version_index = 0;

				while (table[version_index] != nullptr) {
					version_index++;
				}

				table[version_index] = &new_version;

				return true;
			}

			PartialNode* insert_vertex(std::size_t const& position, data_type const& data) {
				if (out_ptrs_size_ < position) {
					throw std::out_of_range("Inserted position is out of edges range");
				}

				PartialNode* next_node_ptr = forward_[position];

				if (next_node_ptr && next_node_ptr->in_ptrs_size_ < next_node_ptr->current_back_pointer_size_) {
					throw std::out_of_range("Position out of second argument node range");
				}

				PartialNode* new_node = new PartialNode<Type>(data, out_ptrs_size_, in_ptrs_size_);

				new_node->forward_[position] = next_node_ptr;
				new_node->backward_[position] = this;

				forward_[position] = new_node;

				current_version++;
				return forward_[position];
			}

			bool update_edge(std::size_t const& position, PartialNode* v) {
				if (out_ptrs_size_ < position) {
					throw std::out_of_range("Position out of first argument node.");
				}

				forward_[position] = v;

				if (next_node_ptr->in_ptrs_size_ < current_back_pointer_size_) {
					throw std::out_of_range("Position out of second argument node range");
				}

				v->backward_[position] = this;

				return true;
			}

			PartialNode& operator[](
				std::pair<std::size_t, unsigned int> const& position_version) const {

				if (out_ptrs_size_ < position_version) {
					throw std::out_of_range("Index out of node edges range.");
				}
				if (in_ptrs_size_ < position_version) {
					throw std::out_of_range("Index out of node edges range.");
				}
				if (!forward_[position_version]) {
					throw std::logic_error("Access to null reference.");
				}
				if (!backward_[position_version]) {
					throw std::logic_error("Access to null reference.");
				}

				if (position_version < 2) {
					return *(table[position_version]->forward);
				}

				return *forward_[position_version-3];

			}

			data_type* data_ = new data_type;
			std::size_t out_ptrs_size_;
			PartialNode** forward_;

			std::size_t in_ptrs_size_;
			std::size_t current_modifications_size_;
			std::size_t current_back_pointer_size_;

			int current_version = 0;
			int next_position = 0;

			PartialNode** backward_;

			std::vector<PartialNode**> table[2];
			// TODO: Table mods, 2 * in_ptrs_size_
		};

		template <typename Type>
		class PartialDirectedGraph {
		public:
			typedef Type data_type;

			PartialDirectedGraph(data_type const data, std::size_t const& out_ptrs_size, std::size_t const& in_ptrs_size) {
				root_ptr_ = new PartialNode<Type>(data, out_ptrs_size, in_ptrs_size);
				in_ptrs_size_ = in_ptrs_size;
				out_ptrs_size_ = out_ptrs_size;
			}

			PartialNode<Type>* get_root_ptr(unsigned int const& version) {
				
				return &(root_ptr_[version]);
			}

			PartialNode<Type> get_root(unsigned int const& version) { 
				return *(root_ptr_[version]); 
			}

			bool add_edge(PartialNode<Type>* u, PartialNode<Type>* v, std::size_t position) {
				++(*current_version_);
				u[position]->forward_ = v;
				v[position]->backward_ = u;
				return true;
			}

		protected:
			unsigned int* current_version_ = 0;
			std::size_t in_ptrs_size_;

			PartialNode<Type>* root_ptr_;
			std::size_t out_ptrs_size_;
		};

	}  // namespace Persistence
}  // namespace ADE

#endif  // SOURCE_PERSISTENCE_PARTIAL_DIRECTED_GRAPH_HPP_
