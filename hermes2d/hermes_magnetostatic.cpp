#include "hermes_magnetostatic.h"
inline double int_u_dvdx_over_x(RealFunction* fu, RealFunction* fv, RefMap* ru, RefMap* rv)
{
    Quad2D* quad = fu->get_quad_2d();

    int o = fu->get_fn_order() + fv->get_fn_order() + ru->get_inv_ref_order() + 1;
    limit_order(o);
    fu->set_quad_order(o);
    fv->set_quad_order(o);

    double *dvdx, *dvdy;
    fv->get_dx_dy_values(dvdx, dvdy);
    double* uval = fu->get_fn_values();
    double* x = ru->get_phys_x(o);

    double result;
    h1_integrate_dd_expression(t_dvdx * uval[i] / x[i]);
    return result;
}

int magnetostatic_bc_types(int marker)
{
    switch (magnetostaticEdge[marker].type)
    {
    case PHYSICFIELDBC_NONE:
        return BC_NONE;
        break;
    case PHYSICFIELDBC_MAGNETOSTATIC_VECTOR_POTENTIAL:
        return BC_ESSENTIAL;
        break;
    case PHYSICFIELDBC_MAGNETOSTATIC_SURFACE_CURRENT:
        return BC_NATURAL;
        break;
    }
}

scalar magnetostatic_bc_values(int marker, double x, double y)
{
    return magnetostaticEdge[marker].value;
}

scalar magnetostatic_bilinear_form(RealFunction* fu, RealFunction* fv, RefMap* ru, RefMap* rv)
{
    int marker = rv->get_active_element()->marker;

    if (magnetostaticIsPlanar)
        return 1.0 / magnetostaticLabel[marker].permeability * int_grad_u_grad_v(fu, fv, ru, rv);
    else
    {
        return 1.0 / magnetostaticLabel[marker].permeability * (int_u_dvdx_over_x(fu, fv, ru, rv) + int_grad_u_grad_v(fu, fv, ru, rv)); 
    }
}

scalar magnetostatic_linear_form(RealFunction* fv, RefMap* rv)
{
    int marker = rv->get_active_element()->marker;

    if (magnetostaticIsPlanar)
        return MU0 * magnetostaticLabel[marker].current_density * int_v(fv, rv);
    else
        return MU0 * magnetostaticLabel[marker].current_density * int_v(fv, rv);
}

SolutionArray magnetostatic_main(const char *fileName, MagnetostaticEdge *edge, MagnetostaticLabel *label, int numberOfRefinements, int polynomialOrder, int adaptivitySteps, bool isPlanar)
{
    magnetostaticEdge = edge;
    magnetostaticLabel = label;
    magnetostaticIsPlanar = isPlanar;

    // save locale
    char *plocale = setlocale (LC_NUMERIC, "");
    setlocale (LC_NUMERIC, "C");

    // load the mesh file
    Mesh mesh;
    mesh.load(fileName);
    for (int i = 0; i < numberOfRefinements; i++)
        mesh.refine_all_elements(0);

    // set system locale
    setlocale(LC_NUMERIC, plocale);

    // initialize the shapeset and the cache
    H1Shapeset shapeset;
    PrecalcShapeset pss(&shapeset);

    // create an H1 space
    H1Space space(&mesh, &shapeset);
    space.set_bc_types(magnetostatic_bc_types);
    space.set_bc_values(magnetostatic_bc_values);
    space.set_uniform_order(polynomialOrder);
    space.assign_dofs();

    // initialize the weak formulation
    WeakForm wf(1);
    wf.add_biform(0, 0, magnetostatic_bilinear_form);
    wf.add_liform(0, magnetostatic_linear_form);
    
// initialize the linear solver
    UmfpackSolver umfpack;
    Solution *sln = new Solution();
    Solution rsln;

    // assemble the stiffness matrix and solve the system
    double error;
    int i;
    for (i = 0; i<(adaptivitySteps+1); i++)
    {
        space.assign_dofs();

        // initialize the linear system
        LinSystem sys(&wf, &umfpack);
        sys.set_spaces(1, &space);
        sys.set_pss(1, &pss);
        sys.assemble();
        sys.solve(1, sln);

        RefSystem rs(&sys);
        rs.assemble();
        rs.solve(1, &rsln);

        // calculate errors and adapt the solution
        if (adaptivitySteps > 0)
        {
            H1OrthoHP hp(1, &space);
            error = hp.calc_error(sln, &rsln) * 100;

            if (error < 0.1 || sys.get_num_dofs() >= NDOF_STOP) break;
            hp.adapt(THRESHOLD, STRATEGY, H_ONLY);
        }
    }

    // order
    space.assign_dofs();
    Orderizer *ord = new Orderizer();
    ord->process_solution(&space);

    return SolutionArray(sln, ord);
}
