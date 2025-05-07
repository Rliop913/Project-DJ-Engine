#include "gitWrapper.hpp"

#include "CommitFinder.hpp"

MAYBE_BLAME
GitWrapper::Blame(const std::string& filepath, const GitCommit& newCommit, const GitCommit& oldCommit)
{
    auto newBlame = BlameController();
    git_blame_options opts;
    git_blame_options_init(&opts, GIT_BLAME_OPTIONS_VERSION);
    opts.newest_commit = newCommit.id;
    opts.oldest_commit = oldCommit.id;
    if(newBlame.BlameOpen(repo, filepath, &opts)){
        return std::move(newBlame);
    }
    else{
        return std::nullopt;
    }
}

DiffResult
GitWrapper::diff(const GitCommit& oldCommit, const GitCommit& newCommit)
{
    auto DiffHandle = DiffController();
    DiffResult results;
    if(!oldCommit.USABLE_FLAG) return results;

    if(newCommit.USABLE_FLAG){
        DiffHandle.CommitToCommit(repo, newCommit.id, oldCommit.id);
    }
    else{
        DiffHandle.CommitToNow(repo, oldCommit.id);
    }
    DiffHandle.execute(&results);
    return results;
}

bool
GitWrapper::add(const std::string& path)
{
    if(addIndex.has_value()){
        addIndex.reset();
    }
    addIndex.emplace();
    if(!addIndex->open(repo)) return false;
    if(!addIndex->addFile(path)) return false;
    return true;
}





bool
GitWrapper::open(const std::string& path)
{
    if(git_repository_open(&repo, path.c_str()) == 0){
        handleBranch.emplace(repo);
        return true;
    }
    else{
        if(git_repository_init(&repo, path.c_str(), false) == 0){
            handleBranch.emplace(repo);
            return true;
        }
        else{
            return false;
        }
    }
}


bool
GitWrapper::close()
{
    if(repo == nullptr){
        return false;
    }
    git_repository_free(repo);
    repo=nullptr;
    return true;
}

GitWrapper::GitWrapper()
{
    git_libgit2_init();
}

GitWrapper::~GitWrapper()
{
    if(repo != nullptr){
        git_repository_free(repo);
    }
    if(addIndex.has_value()) {
        addIndex.reset();
    }
    git_libgit2_shutdown();
}

bool
GitWrapper::commit(git_signature* sign, const std::string& message)
{
    git_oid tree_id, commit_id, parent_id;
    git_tree* tree = nullptr;
    git_commit* parent_commit = nullptr;
    bool result = false;

    if (!addIndex.has_value()) goto cleanup;
    if (git_index_write_tree(&tree_id, addIndex->index) != 0) goto cleanup;
    if (git_tree_lookup(&tree, repo, &tree_id) != 0) goto cleanup;

    // 부모 커밋이 있는 경우
    if (git_reference_name_to_id(&parent_id, repo, "HEAD") == 0 &&
        git_commit_lookup(&parent_commit, repo, &parent_id) == 0) {
        if (git_commit_create_v(
                &commit_id, repo, "HEAD", sign, sign, nullptr,
                message.c_str(), tree, 1, (const git_commit**)&parent_commit
            ) == 0) {
            result = true;
        }
    } else {
        // 최초 커밋(부모 없음)
        if (git_commit_create_v(
                &commit_id, repo, "HEAD", sign, sign, nullptr,
                message.c_str(), tree, 0
            ) == 0) {
            result = true;
        }
    }

cleanup:
    if (tree) git_tree_free(tree);
    if (parent_commit) git_commit_free(parent_commit);
    addIndex.reset();
    return result;
}

// bool
// GitWrapper::MoveToBranch(const std::string& branch_name)
// {
// 	if (!repo) return false;
// 	git_reference* branch_ref = nullptr;
// 	git_object* target = nullptr;
// 	git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
// 	opts.checkout_strategy = GIT_CHECKOUT_SAFE;
// 	bool success = false;

// 	int error = git_branch_lookup(
//         &branch_ref, 
//         repo, 
//         branch_name.c_str(), 
//         GIT_BRANCH_LOCAL);
// 	if (error == GIT_OK) {
// 		// Branch exists, checkout
// 		const git_oid* target_oid = git_reference_target(branch_ref);
// 		if (git_commit_lookup((git_commit**)&target, repo, target_oid) != GIT_OK) {
// 			goto branch_cleanup;
// 		}
// 	} else {
// 		// Branch does not exist, create from HEAD
// 		git_reference* head_ref = nullptr;
// 		git_commit* head_commit = nullptr;
// 		if (git_repository_head(&head_ref, repo) != GIT_OK) goto branch_cleanup;
// 		const git_oid* head_oid = git_reference_target(head_ref);
// 		if (git_commit_lookup(&head_commit, repo, head_oid) != GIT_OK) {
// 			git_reference_free(head_ref);
// 			goto branch_cleanup;
// 		}
// 		if (git_branch_create(&branch_ref, repo, branch_name.c_str(), head_commit, 0) != GIT_OK) {
// 			git_commit_free(head_commit);
// 			git_reference_free(head_ref);
// 			goto branch_cleanup;
// 		}
// 		target = (git_object*)head_commit;
// 		git_reference_free(head_ref);
// 	}
// 	// Checkout the target commit/tree
// 	if (git_checkout_tree(repo, target, &opts) != GIT_OK) goto branch_cleanup;
// 	// Set HEAD to branch
// 	if (git_repository_set_head(repo, (std::string("refs/heads/") + branch_name).c_str()) != GIT_OK) goto branch_cleanup;
// 	success = true;

// branch_cleanup:
// 	if (branch_ref) git_reference_free(branch_ref);
// 	if (target) git_commit_free((git_commit*)target);
// 	return success;
// }



// void merge_cleanup(git_reference* branch_ref, git_commit* their_commit, git_annotated_commit* their_head){
//     if(branch_ref) git_reference_free(branch_ref);
//     if(their_commit) git_commit_free(their_commit);
//     if(their_head) git_annotated_commit_free(their_head);
// }


// bool
// GitWrapper::merge(const std::string& branch)
// {
// 	if (!repo) return false;
// 	git_reference* branch_ref = nullptr;
// 	git_commit* their_commit = nullptr;
// 	git_annotated_commit* their_head = nullptr;
// 	bool success = false;
// 	int branch_lookup_ok = 0;
// 	int commit_lookup_ok = 0;
// 	int annotated_commit_ok = 0;

// 	// 1. 병합할 브랜치 참조 얻기
// 	branch_lookup_ok = git_branch_lookup(&branch_ref, repo, branch.c_str(), GIT_BRANCH_LOCAL);
// 	if (branch_lookup_ok != GIT_OK){
// 		merge_cleanup(branch_ref, their_commit, their_head);
// 		return false;
// 	}
// 	const git_oid* their_oid = git_reference_target(branch_ref);
// 	commit_lookup_ok = git_commit_lookup(&their_commit, repo, their_oid);
// 	if (commit_lookup_ok != GIT_OK){
// 		merge_cleanup(branch_ref, their_commit, their_head);
// 		return false;
// 	}
// 	annotated_commit_ok = git_annotated_commit_lookup(&their_head, repo, their_oid);
// 	if (annotated_commit_ok != GIT_OK){
// 		merge_cleanup(branch_ref, their_commit, their_head);
// 		return false;
// 	}

// 	// 2. 병합 수행
// 	git_merge_options merge_opts = GIT_MERGE_OPTIONS_INIT;
// 	git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;
// 	checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE | GIT_CHECKOUT_RECREATE_MISSING;
// 	git_merge(repo, (const git_annotated_commit**)&their_head, 1, &merge_opts, &checkout_opts);

// 	// 3. 병합 후 상태 확인
// 	if (git_repository_state(repo) == GIT_REPOSITORY_STATE_MERGE) {
// 		// 충돌 발생 등으로 병합 미완료
// 		git_repository_state_cleanup(repo);
// 		merge_cleanup(branch_ref, their_commit, their_head);
// 		return false;
// 	}

// 	success = true;

// 	merge_cleanup(branch_ref, their_commit, their_head);
// 	return success;
// }

// bool
// GitWrapper::checkout(const std::string& branch_name, const std::string& commit_message)
// {
//     if (!repo) return false;
//     git_reference* branch_ref = nullptr;
//     git_commit* target_commit = nullptr;
//     bool success = false;

//     // 1. 브랜치 참조 얻기

//     if(!MoveToBranch(branch_name)) return false;
//     // 2. CommitFinder로 커밋 메시지에 해당하는 커밋 찾기
//     auto finder = GitCommit(commit_message, repo);
    
//     if (!finder.USABLE_FLAG) {
//         return false;
//     }
    
//     if (git_commit_lookup(&target_commit, repo, &finder.id) != GIT_OK) {
//         return false;
//     }

//     // 3. 해당 커밋으로 체크아웃
//     git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
//     opts.checkout_strategy = GIT_CHECKOUT_SAFE | GIT_CHECKOUT_RECREATE_MISSING;
//     if (git_checkout_tree(repo, (git_object*)target_commit, &opts) != GIT_OK) {
//         git_commit_free(target_commit);
//         return false;
//     }

//     // 4. HEAD를 해당 커밋으로 이동 (detached HEAD)
//     if (git_repository_set_head_detached(repo, git_commit_id(target_commit)) != GIT_OK) {
//         git_commit_free(target_commit);
//         return false;
//     }

//     success = true;
//     git_commit_free(target_commit);
//     return success;
// }

// SaveDatas
// GitWrapper::GetCommits()
// {
//     SaveDatas allBranchCommits;
//     git_branch_iterator* it = nullptr;
//     git_reference* ref = nullptr;
//     int error = 0;

//     error = git_branch_iterator_new(&it, repo, GIT_BRANCH_LOCAL);
//     if (error != GIT_OK) return allBranchCommits;

//     while (git_branch_next(&ref, nullptr, it) == GIT_OK) {
//         const char* branch_name = nullptr;
//         if (git_branch_name(&branch_name, ref) != GIT_OK || !branch_name) {
//             git_reference_free(ref);
//             continue;
//         }
//         std::vector<GitCommit> commitContains;
//         git_revwalk* walker = nullptr;
//         if (git_revwalk_new(&walker, repo) != GIT_OK) {
//             git_reference_free(ref);
//             continue;
//         }
//         if (git_revwalk_push_ref(walker, git_reference_name(ref)) != GIT_OK) {
//             git_revwalk_free(walker);
//             git_reference_free(ref);
//             continue;
//         }
//         git_oid oid;
//         while (git_revwalk_next(&oid, walker) == GIT_OK) {
//             GitCommit tempCommit(oid, repo);
//             if(tempCommit.USABLE_FLAG){
//                 commitContains.push_back(tempCommit);
//             }
//         }
//         git_revwalk_free(walker);
//         git_reference_free(ref);
//         allBranchCommits.emplace_back(std::string(branch_name), commitContains);
//     }
//     git_branch_iterator_free(it);
//     return allBranchCommits;
// }